#include <QMessageBox>

#include "transfer/transfer_controller.h"
#include "transfer/transfer_view.h"
#include "transfer/file_transfer.h"


TransferController::TransferController(TransferView *view,
                                       FileTransfer *transfer)
    : QObject(), ObserverBase(), view_(view),
      file_transfer_(transfer), actived_(false)
{
    connect(view->ui_.pushButtonOpen, &QPushButton::clicked,
            this, &TransferController::OnOpenFile);
    connect(view->ui_.pushButtonSend, &QPushButton::clicked,
            this, &TransferController::OnTransferFile);

    view_->SetProgressBarStatus(false);
    view_->SetTransferButtonText("Transmit");
    view_->SetTransferButtonStatus(false);
    view_->SetModel(file_transfer_);
    file_transfer_->AddObserver(this);
}

TransferController::~TransferController()
{
    delete file_transfer_;
    delete view_;
}

void TransferController::OnOpenFile()
{
    QString *filename = file_transfer_->filename();
    bool okay;
    okay = view_->OpenFileSystem(filename, "File(*.bin)");
    if (okay) {
        view_->ResetProgressBar();
        view_->SetTransferButtonStatus(true);
        view_->ShowTransferFile(*filename);
    }
}

void TransferController::OnTransferFile()
{
    if (!actived_) {
        if (file_transfer_->StartFileTransfer()) {
            view_->ResetProgressBar();
            view_->SetTransferButtonText("Stop");
            actived_ = true;
        }
    } else {
        file_transfer_->StopFileTransfer();
        view_->SetTransferButtonText("Transmit");
        actived_ = false;
    }
}

void TransferController::Update(int action, void *ptr)
{
    switch (action) {
    case FileTransfer::TRANSFER_SHOW_PROGRESS:
        view_->ShowTransferProcess(*(quint32 *)ptr);
        break;
    case FileTransfer::TRANSFER_SHOW_TX_PACKET: {
        const QByteArray *ba = (const QByteArray *)ptr;
        QByteArray str(ba->toHex(' ').toUpper());
        view_->ShowText("Send Packet: ");
        view_->ShowText(str);
        }
        break;
    case FileTransfer::TRANSFER_SHOW_RX_PACKET: {
        const QByteArray *ba = (const QByteArray *)ptr;
        QByteArray str(ba->toHex(' ').toUpper());
        view_->ShowText("Receive Packet: ");
        view_->ShowText(str);
        }
        break;
    case FileTransfer::TRANSFER_SHOW_RETX_PACKET: {
        const QByteArray *ba = (const QByteArray *)ptr;
        QByteArray str(ba->toHex(' ').toUpper());
        view_->ShowText("Resend Packet: ");
        view_->ShowText(str);
        }
        break;
    case FileTransfer::TRANSFER_FINISHED: {
        OnTransferFile();
        QString crc = QString::number(file_transfer_->crc(), 16);
        QString size = QString::number(file_transfer_->size());
        view_->ShowText("File SIZE: " + size);
        view_->ShowText("File CRC: " + crc);
        view_->ShowText("File transfer completed!");
        }
        break;
    case FileTransfer::TRANSFER_FAILED:
        view_->ShowText("File transfer failed\n");
        break;
    case FileTransfer::TRANSFER_PORT_CLOSED:
        QMessageBox::information(NULL,
                                 "Warnning",
                                 "Please select a port");
        break;
    }
}

void TransferController::Show()
{
    view_->showNormal();
}

void TransferController::MessageProcess(QByteArray *buf)
{
    file_transfer_->FileMessageProcess(buf);
}

