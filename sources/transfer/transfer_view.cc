#include <QFileDialog>
#include <QPushButton>
#include <QLineEdit>
#include <QProgressBar>
#include <QTextBrowser>
#include <QString>

#include "transfer/transfer_view.h"
#include "transfer/file_transfer.h"

TransferView::TransferView(QWidget *parent, QTextBrowser *text_browser)
    : QWidget(parent), text_browser_(text_browser)
{
    ui_.setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::Popup);
}

TransferView::~TransferView()
{

}

bool TransferView::OpenFileSystem(QString *filename,
                                  const QString &filter,
                                  const QString &title)
{
    if (filename == nullptr)
        return false;

    QFileDialog fs(this);
    if (filename->isEmpty())
        fs.setDirectory(".");
    else
        fs.setDirectory(*filename);
    fs.setWindowTitle(title);
    fs.setNameFilter(filter);
    fs.setFileMode(QFileDialog::ExistingFiles);
    fs.setViewMode(QFileDialog::Detail);
    if (!fs.exec()) {
        filename->clear();
        return false;
    }
    QStringList list = fs.selectedFiles();
    if (list.isEmpty())
        return false;

    *filename = list.takeFirst();
    return true;
}

void TransferView::ShowTransferFile(const QString &name)
{
    ui_.lineEditFileName->setText(name);
}

void TransferView::ShowTransferProcess(quint32 percent)
{
    ui_.progressBar->setValue(percent);
}

void TransferView::SetTransferButtonStatus(bool enable)
{
    ui_.pushButtonSend->setEnabled(enable);
}

void TransferView::ResetProgressBar()
{
    ui_.progressBar->setRange(0, 100);
    ui_.progressBar->setValue(0);
    SetProgressBarStatus(true);
}

void TransferView::SetTransferButtonText(const QString &text)
{
    ui_.pushButtonSend->setText(text);
}

void TransferView::SetProgressBarStatus(bool visible)
{
    ui_.progressBar->setVisible(visible);
}

void TransferView::ShowText(const QString &text)
{
    text_browser_->append(text);
}

void TransferView::closeEvent(QCloseEvent* ev)
{
    QWidget::closeEvent(ev);
    model_->CloseProcess();
}

void TransferView::SetModel(FileTransfer *model)
{
    model_ = model;
}

bool TransferView::DebugEnabled() const
{
    return ui_.checkbox_debug_->isChecked();
}
