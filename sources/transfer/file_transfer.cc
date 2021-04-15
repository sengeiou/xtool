#include <QFile>
#include <QObject>
#include <QByteArray>
#include <QTimer>

#include "transfer/file_transfer.h"
#include "stp/stp.h"
#include "xtool_model.h"

#ifdef CONFIG_PROTOBUF
#include "protobuf/ota.pb.h"
#endif

//File transfer subcommand
#define OTA_FILE_REQ  0x01
#define OTA_FILE_BKPT 0x02
#define OTA_FILE_DATA 0x03
#define OTA_FILE_CMP  0x04


FileTransfer::FileTransfer(XToolModel *master)
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

void FileTransfer::CloseProcess()
{
    master_->ResumeMessageProcess();
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
        Notify(FileTransfer::TRANSFER_SHOW_RETX_PACKET, txbuf_);
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

    Notify(FileTransfer::TRANSFER_SHOW_RX_PACKET, buf);
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
        Notify(FileTransfer::TRANSFER_SHOW_TX_PACKET, txbuf_);
        return true;
    }

    Notify(FileTransfer::TRANSFER_PORT_CLOSED, nullptr);
    return false;
}

bool FileTransfer::GenerateReceive(QByteArray *buf, quint8 match_code)
{
    quint16 pkglen;
    const quint8 *packet = stp_->ToL2(*buf, &pkglen);
    if (packet[0] != STP_OTA_CLASS)
        return false;

    if (pkglen <= 4)
        return false;

    StpL3Header *pkg = (StpL3Header *)&packet[1];
    if (pkg->minor != match_code)
        return false;

    if (!(Netbuffer::ToCpu16(pkg->length) & OPC_SCODE))
        return false;

    if (pkg->data[0] != 0)
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
    const StpL3Header *hdr;
    const quint8 *packet;

    packet = context->stp_->ToL2(*buf, nullptr);
    if (packet[0] != STP_OTA_CLASS)
        return false;

    hdr = (const StpL3Header *)&packet[1];
    if (hdr->minor != OTA_FILE_REQ)
        return false;

#ifdef CONFIG_PROTOBUF
    ota::FileRequest req;
    bool okay = req.ParseFromArray(hdr->data, OPC_LEN(Netbuffer::ToCpu16(hdr->length)));
    if (!okay || req.battery() < 80)
        return false;
#else
    if (OPC_LEN(Netbuffer::ToCpu16(hdr->length)) != sizeof(*sta))
        return false;

    const FileRequestStatus *sta = (const FileRequestStatus *)hdr->data;
    //quint16 fsize = Netbuffer::ToCpu16(sta->max_file_size);
    if (sta->bat_power < 80)
        return false;
#endif
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
    const quint8 *packet;
    packet = context->stp_->ToL2(*buf, nullptr);
    if (packet[0] != STP_OTA_CLASS)
        return false;

    const StpL3Header *hdr = (const StpL3Header *)&packet[1];
    if (hdr->minor != OTA_FILE_BKPT)
        return false;

    OTAHeader *ota = context->ota_;

#ifdef CONFIG_PROTOBUF
    ota::File ota_file;
    bool okay = ota_file.ParseFromArray(hdr->data, OPC_LEN(Netbuffer::ToCpu16(hdr->length)));
    if (!okay || ota->maxno != ota_file.maxno()) {
        context->SetTranferState(&context->send_state_);
        return context->SendProcess();
    }

    QFile *file = context->file_;
    QByteArray ba = file->readAll();
    quint16 no = (quint16)ota_file.no();
    quint32 file_crc = context->FileChecksum((const quint8 *)ba.constData(),
                                     no * OTAHeader::MAX_PAYLOAD,
                                     0);
    if (file_crc == ota_file.crc()) {
        ota->seqno = no + 1;
        context->file_crc_ = file_crc;
        context->file_sent_size_ = no * OTAHeader::MAX_PAYLOAD;
        file->seek(context->file_sent_size_);
    } else {
        file->seek(0);
    }
#else

    quint16 len = Netbuffer::ToCpu16(hdr->length);
    if (len == 0) {
        context->SetTranferState(&context->send_state_);
        return context->SendProcess();
    }
    if (len != sizeof(*fbkpt))
        return false;

    const FileTransferContext *fbkpt = (const FileTransferContext *)hdr->data;
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
#endif
    context->SetTranferState(&context->send_state_);
    return context->SendProcess();
}

//Send file
bool FileSendState::Send(FileTransfer *context)
{
    QByteArray *txbuf = context->txbuf_;
    StpOpcode *stp = context->stp_;
    QFile *file = context->file_;
    qint64 ret;
    quint16 pkglen;

    // Send completed
    if (file->atEnd()) {
        context->file_size_ = file->size();
        context->CloseFile();
        context->SetTranferState(&context->stop_state_);
        return context->SendProcess();
    }

#ifdef CONFIG_PROTOBUF
    char buffer[OTAHeader::MAX_PAYLOAD + 16];
    //Read error
    ret = file->read(buffer, OTAHeader::MAX_PAYLOAD);
    if (ret < 0) {
        context->CloseFile();
        return false;
    }
    context->file_crc_ = context->FileChecksum((const quint8 *)buffer,
            (quint32)ret, context->file_crc_);

    ota::File ota_file;
    ota_file.set_data(buffer, ret);
    ota_file.set_maxno(context->ota_->maxno);
    ota_file.set_no(context->ota_->seqno);
    ota_file.SerializeToArray(buffer, ota_file.ByteSizeLong());
    pkglen = (quint16)ota_file.ByteSizeLong();
#else
    char buffer[4 + OTAHeader::MAX_PAYLOAD];
    //Read error
    ret = file->read(&buffer[4], OTAHeader::MAX_PAYLOAD);
    if (ret < 0) {
        context->CloseFile();
        return false;
    }

    context->file_crc_ = context->FileChecksum((const quint8 *)&buffer[4],
            (quint32)ret, context->file_crc_);
    OTAHeader *ota = reinterpret_cast<OTAHeader *>(buffer);
    ota->maxno = Netbuffer::ToNet16(context->ota_->maxno);
    ota->seqno = Netbuffer::ToNet16(context->ota_->seqno);
    pkglen = ret + sizeof(OTAHeader);
#endif
    context->file_sent_size_ += ret;
    context->ota_->seqno++;
    context->file_percent_ = context->ota_->seqno * 100 / context->ota_->maxno;
    context->Notify(FileTransfer::TRANSFER_SHOW_PROGRESS,
                              &context->file_percent_);
    stp->AppendMessage(OTA_FILE_DATA, buffer, OPC_DLEN(pkglen));
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

#ifdef CONFIG_PROTOBUF
    ota::FileCheck file_chk;
    char buffer[10];
    file_chk.set_crc(context->file_crc_);
    quint16 pkglen = (quint16)file_chk.ByteSizeLong();
    file_chk.SerializeToArray(buffer, pkglen);
    stp->AppendMessage(OTA_FILE_CMP, buffer, OPC_DLEN(pkglen));
#else
    quint32 crc = Netbuffer::ToNet32(context->file_crc_);
    stp->AppendMessage(OTA_FILE_CMP, (const char *)&crc, sizeof(crc));
#endif
    stp->GeneratePacket(STP_OTA_CLASS, 0, txbuf);
    return context->master_->Send(*txbuf);
}

bool FileStopState::Receive(FileTransfer *context, QByteArray *buf)
{
    if (context->GenerateReceive(buf, OTA_FILE_CMP)) {
        context->Notify(FileTransfer::TRANSFER_FINISHED, nullptr);
        return true;
    }

    context->Notify(FileTransfer::TRANSFER_FAILED, nullptr);
    return false;
}

