#include <QFile>
#include <QObject>
#include <QByteArray>
#include <QTimer>

#include "transfer/file_transfer.h"
#include "stp.h"
#include "xtoolform.h"

//File transfer subcommand
#define OTA_FILE_REQ  0x01
#define OTA_FILE_BKPT 0x02
#define OTA_FILE_DATA 0x03
#define OTA_FILE_CMP  0x04


FileTransfer::FileTransfer(XToolForm *master)
      : master_(master),
      file_(nullptr),
      txbuf_(new QByteArray),
      timer_(new QTimer),
      ota_(new OTAHeader),
      stp_(new StpOpcode)
{
    connect(timer_, &QTimer::timeout, this, &FileTransfer::OnTimeout);
}

FileTransfer::~FileTransfer()
{
    CloseFile();
    delete timer_;
    delete txbuf_;
    delete stp_;
    delete ota_;
}

bool FileTransfer::StartFileTransfer()
{
    if (filename_.isEmpty())
        return false;

    QFile *file = file_;
    if (file == nullptr) {
        file = new QFile(filename_);
        if (!file->open(QFile::ReadOnly)) {
            delete file;
            return false;
        }
        file_ = file;
    }

    ResetOTAHeader(*file);
    file_crc_ = 0;
    file_sent_size_ = 0;
    SetTranferState(&requst_state_);
    return SendProcess();
}

void FileTransfer::StopFileTransfer()
{
    if (timer_->isActive())
        timer_->stop();

    SetTranferState(&null_state_);
    CloseFile();
}

void FileTransfer::CloseFile()
{
    QFile *file = file_;
    if (file) {
        file_ = nullptr;
        file->close();
        delete file;
    }
}

void FileTransfer::ResetOTAHeader(const QFile &file)
{
    ota_->seqno = 1;
    ota_->maxno = static_cast<quint16>(file.size() / OTAHeader::MAX_PAYLOAD);
    if (file.size() & (OTAHeader::MAX_PAYLOAD - 1))
     ota_->maxno++;
}

void FileTransfer::OnTimeout()
{
    if (tx_retry_ > 0) {
        observer_.Notify(FileTransfer::TRANSFER_SHOW_RETX_PACKET, txbuf_);
        master_->Send(*txbuf_);
        tx_retry_--;
    } else {
        CloseFile();
    }
}

inline bool FileTransfer::ReceiveProcess(QByteArray *buf)
{
    return transfer_->Receive(this, buf);
}

void FileTransfer::FileMessageProcess(QByteArray *buf)
{
    if (timer_->isActive())
        timer_->stop();

    observer_.Notify(FileTransfer::TRANSFER_SHOW_RX_PACKET, buf);
    if (stp_->ProcessMessage(*buf)) {
        if (ReceiveProcess(buf))
            return;
    }
    timer_->start(3000);
}

quint32 FileTransfer::FileChecksum(const quint8 *data,
                                   quint32 len,
                                   quint32 crc)
{
    crc = ~crc;
    for (size_t i = 0; i < len; i++) {
        crc = crc ^ data[i];
        for (quint8 j = 0; j < 8; j++) {
            crc = (crc >> 1) ^ (0xEDB88320 & -(crc & 1));
        }
    }
    return (~crc);
}

void FileTransfer::SetTranferState(AbstractTransfer *next_state)
{
    transfer_ = next_state;
}

bool FileTransfer::SendProcess(void)
{
    txbuf_->clear();
    stp_->Reset();
    if (transfer_->Send(this)) {
        tx_retry_ = 2;
        timer_->setSingleShot(true);
        timer_->start(3000);
        observer_.Notify(FileTransfer::TRANSFER_SHOW_TX_PACKET, txbuf_);
        return true;
    }

    observer_.Notify(FileTransfer::TRANSFER_PORT_CLOSED, nullptr);
    return false;
}

bool FileTransfer::GenerateReceive(QByteArray *buf, quint8 match_code)
{
    quint16 len;
    const quint8 *packet = stp_->ToL2(*buf, &len);
    if (packet[0] != STP_OTA_CLASS)
        return false;
    if (len != 3)
        return false;
    if (packet[1] != match_code || packet[2] != 0)
        return false;
    return true;
}

//Request
bool FileRequestState::Send(FileTransfer *context)
{
    QByteArray *txbuf = context->txbuf_;
    StpOpcode *stp = context->stp_;
    stp->AppendMessage(OTA_FILE_REQ, nullptr, 0);
    stp->GeneratePacket(STP_OTA_CLASS, 0, txbuf);
    return context->master_->Send(*txbuf);
}

bool FileRequestState::Receive(FileTransfer *context, QByteArray *buf)
{
    const FileRequestStatus *sta;
    const StpL3Header *hdr;
    const quint8 *packet;

    packet = context->stp_->ToL2(*buf, nullptr);
    if (packet[0] != STP_OTA_CLASS)
        return false;

    hdr = (const StpL3Header *)&packet[1];
    if (hdr->minor != OTA_FILE_REQ)
        return false;

    if (Netbuffer::ToCpu16(hdr->length) != sizeof(*sta))
        return false;

    sta = (const FileRequestStatus *)hdr->data;
    //quint16 fsize = Netbuffer::ToCpu16(sta->max_file_size);
    if (sta->bat_power < 80)
        return false;

    context->SetTranferState(&context->bkpt_state_);
    return context->SendProcess();
}

//Breakpoint information
bool FileBreakpointState::Send(FileTransfer *context)
{
    QByteArray *txbuf = context->txbuf_;
    StpOpcode *stp = context->stp_;
    stp->AppendMessage(OTA_FILE_BKPT, nullptr, 0);
    stp->GeneratePacket(STP_OTA_CLASS, 0, txbuf);
    return context->master_->Send(*txbuf);
}

bool FileBreakpointState::Receive(FileTransfer *context, QByteArray *buf)
{
    const FileTransferContext *fbkpt;
    const quint8 *packet;

    packet = context->stp_->ToL2(*buf, nullptr);
    if (packet[0] != STP_OTA_CLASS)
        return false;

    const StpL3Header *hdr = (const StpL3Header *)&packet[1];
    if (hdr->minor != OTA_FILE_BKPT)
        return false;

    quint16 len = Netbuffer::ToCpu16(hdr->length);
    if (len == 0) {
        context->SetTranferState(&context->send_state_);
        return context->SendProcess();
    }
    if (len != sizeof(*fbkpt))
        return false;

    OTAHeader *ota = context->ota_;
    fbkpt = (const FileTransferContext *)hdr->data;
    quint16 maxno = Netbuffer::ToCpu16(fbkpt->maxno);
    if (ota->maxno != maxno) {
        context->SetTranferState(&context->send_state_);
        return context->SendProcess();
    }

    QFile *file = context->file_;
    QByteArray ba = file->readAll();
    quint16 no = Netbuffer::ToCpu16(fbkpt->no);
    quint32 file_crc = context->FileChecksum((const quint8 *)ba.constData(),
                                     no * OTAHeader::MAX_PAYLOAD,
                                     0);
    if (file_crc == Netbuffer::ToCpu32(fbkpt->crc)) {
        ota->seqno = no + 1;
        context->file_crc_ = file_crc;
        context->file_sent_size_ = no * OTAHeader::MAX_PAYLOAD;
        file->seek(context->file_sent_size_);
    } else {
        file->seek(0);
    }

    context->SetTranferState(&context->send_state_);
    return context->SendProcess();
}

//Send file
bool FileSendState::Send(FileTransfer *context)
{
    QByteArray *txbuf = context->txbuf_;
    StpOpcode *stp = context->stp_;
    QFile *file = context->file_;
    char buffer[4 + OTAHeader::MAX_PAYLOAD];
    OTAHeader *ota;
    qint64 ret;

    // Send completed
    if (file->atEnd()) {
        context->file_size_ = file->size();
        context->CloseFile();
        context->SetTranferState(&context->stop_state_);
        return context->SendProcess();
    }

    //Read error
    ret = file->read(&buffer[4], OTAHeader::MAX_PAYLOAD);
    if (ret < 0) {
        context->CloseFile();
        return false;
    }

    context->file_sent_size_ += ret;
    context->file_crc_ = context->FileChecksum((const quint8 *)&buffer[4],
            (quint32)ret, context->file_crc_);
    ota = reinterpret_cast<OTAHeader *>(buffer);
    ota->maxno = Netbuffer::ToNet16(context->ota_->maxno);
    ota->seqno = Netbuffer::ToNet16(context->ota_->seqno);
    context->ota_->seqno++;
    context->file_percent_ = context->ota_->seqno * 100 / context->ota_->maxno;
    context->observer_.Notify(FileTransfer::TRANSFER_SHOW_PROGRESS,
                              &context->file_percent_);
    stp->AppendMessage(OTA_FILE_DATA, buffer, ret+sizeof(OTAHeader));
    stp->GeneratePacket(STP_OTA_CLASS, 0, txbuf);
    return context->master_->Send(*txbuf);
}

bool FileSendState::Receive(FileTransfer *context, QByteArray *buf)
{
    if (context->GenerateReceive(buf, OTA_FILE_DATA))
        return context->SendProcess();
    return false;
}

//Send completed
bool FileStopState::Send(FileTransfer *context)
{
    QByteArray *txbuf = context->txbuf_;
    StpOpcode *stp = context->stp_;
    quint32 crc = Netbuffer::ToNet32(context->file_crc_);
    stp->AppendMessage(OTA_FILE_CMP, (const char *)&crc, sizeof(crc));
    stp->GeneratePacket(STP_OTA_CLASS, 0, txbuf);
    return context->master_->Send(*txbuf);
}

bool FileStopState::Receive(FileTransfer *context, QByteArray *buf)
{
    if (context->GenerateReceive(buf, OTA_FILE_CMP)) {
        context->observer_.Notify(FileTransfer::TRANSFER_FINISHED, nullptr);
        return true;
    }

    context->observer_.Notify(FileTransfer::TRANSFER_FAILED, nullptr);
    return false;
}

