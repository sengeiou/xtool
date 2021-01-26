#include <QtUiTools>
#include <QFile>
#include <QPushButton>
#include <QLineEdit>
#include <QProgressBar>
#include <QMessageBox>
#include <QByteArray>
#include <QTimer>

#include "transferform.h"
#include "ui_transferform.h"

#include "xtoolform.h"
#include "dlistbuf.h"
#include "stp.h"

//File transfer subcommand
#define OTA_FILE_REQ  0x01
#define OTA_FILE_BKPT 0x02
#define OTA_FILE_DATA 0x03
#define OTA_FILE_CMP  0x04


TransferForm::TransferForm(XToolForm *xtool, QWidget *parent)
    : QWidget(parent),
      ui_(new Ui::TransferForm),
      master_(xtool),
      file_(nullptr),
      txbuf_(new QByteArray),
      timer_(new QTimer),
      ota_(new OTAHeader),
      stp_(new StpOpcode)
{
    ui_->setupUi(this);

    //File transmit group
    btn_open_ = ui_->pushButtonOpen; //findChild<QPushButton *>("pushButtonOpen");
    btn_send_ = ui_->pushButtonSend; //findChild<QPushButton *>("pushButtonSend");
    fname_ledit_ = ui_->lineEditFileName; //findChild<QLineEdit *>("lineEditFileName");
    percent_bar_ = ui_->progressBar; //findChild<QProgressBar *>("progressBar");

    percent_bar_->setVisible(false);
    btn_send_->setEnabled(false);

    connect(btn_open_, &QPushButton::clicked, this, &TransferForm::OnOpenFile);
    connect(btn_send_, &QPushButton::clicked, this, &TransferForm::OnSendFile);

    //Timer
    connect(timer_, &QTimer::timeout,
            this, &TransferForm::OnTimeout);

    Qt::WindowFlags flags = windowFlags();
    flags &= ~(Qt::WindowMaximizeButtonHint | Qt::WindowMinMaxButtonsHint);
    setWindowFlags(flags);
    setFixedSize(width(),height());
    setWindowTitle("File transfer");
    txbuf_->resize(512);
    text_browser_ = master_->text_browser_;
    btn_tranfer_status_ = false;
}

TransferForm::~TransferForm()
{
    if (file_)
        delete file_;
    delete timer_;
    delete txbuf_;
    delete stp_;
    delete ota_;
    delete ui_;
}

void TransferForm::closeEvent(QCloseEvent* ev)
{
    QWidget::closeEvent(ev);
    master_->ResumeMessageProcess();
}

void TransferForm::OnOpenFile()
{
    QFileDialog fdlg(this);
    fdlg.setWindowTitle("Select File");
    if (filename_.isEmpty())
        fdlg.setDirectory(".");
    else
        fdlg.setDirectory(filename_);

    fdlg.setNameFilter("File(*.bin)");
    fdlg.setFileMode(QFileDialog::ExistingFiles);
    fdlg.setViewMode(QFileDialog::Detail);
    if (fdlg.exec()) {
        QStringList list = fdlg.selectedFiles();
        if (!list.isEmpty()) {
            filename_ = list.takeFirst();
            fname_ledit_->setText(filename_);
            btn_send_->setEnabled(true);
        }
    }
}

void TransferForm::CloseFile()
{
    text_browser_->append("Close File\n");
    QFile *file = file_;
    if (file) {
        file_ = nullptr;
        file->close();
        delete file;
    }
}

void TransferForm::ResetOTAHeader(const QFile &file)
{
    ota_->seqno = 1;
    ota_->maxno = static_cast<quint16>(file.size() / OTAHeader::MAX_PAYLOAD);
    if (file.size() & (OTAHeader::MAX_PAYLOAD - 1))
     ota_->maxno++;
}

void TransferForm::OnTimeout()
{
    if (tx_retry_ > 0) {
        ShowPacket("Resend Packet: ", *txbuf_);
        master_->Send(*txbuf_);
        tx_retry_--;
    } else {
        percent_bar_->setVisible(false);
        CloseFile();
    }
}

inline bool TransferForm::ReceiveProcess(QByteArray *buf)
{
    return transfer_->Receive(this, buf);
}

void TransferForm::FileMessageProcess(QByteArray *buf)
{
    if (timer_->isActive())
        timer_->stop();

    ShowPacket("Received Packet: ", *buf);
    if (stp_->ProcessMessage(*buf)) {
        if (ReceiveProcess(buf))
            return;
    }
    timer_->start(3000);
}

void TransferForm::OnSendFile()
{
    if (!btn_tranfer_status_) {
        if (filename_.isEmpty())
            return;
        if (!file_) {
            file_ = new QFile(filename_);
            if (!file_->open(QFile::ReadOnly)) {
                QFile *file = file_;
                file_ = nullptr;
                delete file;
                QMessageBox::information(NULL, "Open failed", filename_);
                return;
            }

            ResetOTAHeader(*file_);
            percent_bar_->setRange(0, 100);
            percent_bar_->setValue(0);
            percent_bar_->setVisible(true);
            file_crc_ = 0;
            file_sent_size_ = 0;
        }

        SetTranferState(&requst_state_);
        btn_tranfer_status_ = SendProcess();
        if (btn_tranfer_status_)
            btn_send_->setText("Stop");
    } else {
        if (timer_->isActive())
            timer_->stop();
        SetTranferState(&null_state_);
        btn_send_->setText("Transmit");
        btn_tranfer_status_ = false;
        CloseFile();
    }
}

void TransferForm::ShowPacket(const QString &title, const QByteArray &text)
{
    QByteArray str(text.toHex(' ').toUpper());
    master_->text_browser_->append(title);
    master_->text_browser_->append(str);
}

quint32 TransferForm::FileChecksum(const quint8 *data,
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

void TransferForm::SetTranferState(AbstractTransfer *next_state)
{
    transfer_ = next_state;
}

bool TransferForm::SendProcess(void)
{
    txbuf_->clear();
    stp_->Reset();
    bool okay = transfer_->Send(this);
    tx_retry_ = 2;
    timer_->setSingleShot(true);
    timer_->start(3000);
    ShowPacket("Send Packet: ", *txbuf_);
    return okay;
}


bool TransferForm::GenerateReceive(QByteArray *buf, quint8 match_code)
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
bool FileRequestState::Send(TransferForm *context)
{
    QByteArray *txbuf = context->txbuf_;
    StpOpcode *stp = context->stp_;
    stp->AppendMessage(OTA_FILE_REQ, nullptr, 0);
    stp->GeneratePacket(STP_OTA_CLASS, 0, txbuf);
    return context->master_->Send(*txbuf);
}

bool FileRequestState::Receive(TransferForm *context, QByteArray *buf)
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
bool FileBreakpointState::Send(TransferForm *context)
{
    QByteArray *txbuf = context->txbuf_;
    StpOpcode *stp = context->stp_;
    stp->AppendMessage(OTA_FILE_BKPT, nullptr, 0);
    stp->GeneratePacket(STP_OTA_CLASS, 0, txbuf);
    return context->master_->Send(*txbuf);
}

bool FileBreakpointState::Receive(TransferForm *context, QByteArray *buf)
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
bool FileSendState::Send(TransferForm *context)
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
    context->percent_bar_->setValue(context->ota_->seqno * 100 / context->ota_->maxno);
    stp->AppendMessage(OTA_FILE_DATA, buffer, ret+sizeof(OTAHeader));
    stp->GeneratePacket(STP_OTA_CLASS, 0, txbuf);
    return context->master_->Send(*txbuf);
}

bool FileSendState::Receive(TransferForm *context, QByteArray *buf)
{
    if (context->GenerateReceive(buf, OTA_FILE_DATA))
        return context->SendProcess();
    return false;
}

//Send completed
bool FileStopState::Send(TransferForm *context)
{
    QByteArray *txbuf = context->txbuf_;
    StpOpcode *stp = context->stp_;
    quint32 crc = Netbuffer::ToNet32(context->file_crc_);
    stp->AppendMessage(OTA_FILE_CMP, (const char *)&crc, sizeof(crc));
    stp->GeneratePacket(STP_OTA_CLASS, 0, txbuf);
    return context->master_->Send(*txbuf);
}

bool FileStopState::Receive(TransferForm *context, QByteArray *buf)
{
    if (context->GenerateReceive(buf, OTA_FILE_CMP)) {
        QString str_crc = QString::number(context->file_crc_, 16);
        QString str_size = QString::number(context->file_size_);
        context->text_browser_->append("File SIZE: " + str_size);
        context->text_browser_->append("File CRC: " + str_crc);
        context->text_browser_->append("File transfer completed!");
        goto _exit;
    }
    context->text_browser_->append("File transfer failed\n");

_exit:
    context->OnSendFile();
    return true;
}
