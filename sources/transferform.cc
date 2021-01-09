#include <QtUiTools>
#include <QFile>
#include <QPushButton>
#include <QLineEdit>
#include <QProgressBar>

#include "transferform.h"

TransferForm::TransferForm(QWidget *parent)
    : QWidget(parent)
{
    QUiLoader loader;

    QFile file(":/forms/transferform.ui");
    if (!file.open(QFile::ReadOnly)) {
        //logger()->debug(tr("Failed to open UI file."));
        return;
    }

    QWidget *widget = loader.load(&file, this);
    file.close();
    if (widget == nullptr)
        return;

    //File transmit group
    btn_open_ = findChild<QPushButton *>("pushButtonOpen");
    btn_send_ = findChild<QPushButton *>("pushButtonSend");
    fname_ledit_ = findChild<QLineEdit *>("lineEditFileName");
    percent_bar_ = findChild<QProgressBar *>("progressBar");

    percent_bar_->setVisible(false);
    btn_send_->setEnabled(false);

    connect(btn_open_, &QPushButton::clicked, this, &TransferForm::OnOpenFile);
    connect(btn_send_, &QPushButton::clicked, this, &TransferForm::OnSendFile);

    Qt::WindowFlags flags = windowFlags();
    flags &= ~(Qt::WindowMaximizeButtonHint | Qt::WindowMinMaxButtonsHint);
    setWindowFlags(flags);
    setFixedSize(widget->width(),widget->height());
    setWindowTitle("File transfer");
}

void TransferForm::OnOpenFile()
{
    QFileDialog fdlg(this);

    fdlg.setWindowTitle("Select File");
    if (filename_.isEmpty())
        fdlg.setDirectory(".");
    else
        fdlg.setDirectory(filename_);

    //Filter file name
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

void TransferForm::OnSendFile()
{
    percent_bar_->setVisible(true);
}
