#include <QtUiTools>
#include <QSpinBox>
#include <QLabel>
#include <QVBoxLayout>
#include <QFile>
#include <QAction>
#include <QPushButton>
#include <QLineEdit>
#include <QFileDialog>
#include <QListWidget>
#include <QTextBrowser>
#include <QByteArray>
#include <QTimer>

#include "xtoolform.h"
#include "serialform.h"
#include "transferform.h"
#include "xmlparse.h"
#include "xmlwidget.h"
#include "stp.h"
#include "serial.h"


XToolForm::XToolForm(QWidget *parent)
    : QMainWindow(parent),
      serial_(nullptr),
      xml_(nullptr),
      stp_(new StpOpcode()),
      timer_(new QTimer)
{
    QUiLoader loader;

    QFile file(":/forms/xtoolform.ui");
    if (!file.open(QFile::ReadOnly)) {
        //logger()->debug(tr("Failed to open UI file."));
        return;
    }

    QWidget *widget = loader.load(&file, this);
    file.close();
    if (widget == nullptr)
        return;

    //File menu
    open_ = findChild<QAction *>("actionOpen");
    exit_ = findChild<QAction *>("actionExit");

    //Port menu
    conn_ = findChild<QAction *>("actionConnect");

    //About
    about_ = findChild<QAction *>("actionAbout");

    //File transmit group
    transfer_ = findChild<QAction *>("actionSend_File");

    list_widget_ = findChild<QListWidget *>("listWidget");
    text_browser_ = findChild<QTextBrowser *>("textBrowser");

    //Connect events
    connect(exit_, &QAction::triggered,
            this, &QMainWindow::close);
    connect(conn_, &QAction::triggered,
            this, &XToolForm::OnActionConnect);
    connect(transfer_, &QAction::triggered,
            this, &XToolForm::OnActionTransfer);
    connect(open_, &QAction::triggered,
            this, &XToolForm::OnOpenFile);
    connect(list_widget_, &QListWidget::itemClicked,
            this, &XToolForm::OnListActived);

    //Set right-click menu for list-widget item
    list_widget_->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(list_widget_, &QListWidget::customContextMenuRequested,
            this, &XToolForm::ProvideContextMenu);

    //Timer
    connect(timer_, &QTimer::timeout, this, &XToolForm::OnTimeout);

    widget->setWindowFlags(Qt::SubWindow);
    setWindowTitle(tr("xtool"));
    setWindowIcon(QIcon(":/images/main_icon.png"));
    //setCentralWidget(widget);
}

XToolForm::~XToolForm()
{
    ClosePort();
    if (xml_) {
        delete xml_;
        xml_ = nullptr;
    }
    delete stp_;
    stp_ = nullptr;
}

bool XToolForm::SendMessage(const QByteArray &buf)
{
    if (serial_)
        return serial_->SendTo(buf);
    return false;
}

void XToolForm::ClosePort()
{
    if (serial_) {
        delete serial_;
        serial_ = nullptr;
    }
}

void XToolForm::OnActionConnect()
{
    SerialForm *form = new SerialForm();
    form->SetMasterForm(this);
    form->showNormal();
}

void XToolForm::OnActionTransfer()
{
    TransferForm *form = new TransferForm();
    form->showNormal();
}

void XToolForm::OnOpenFile()
{
    QFileDialog fdlg(this);
    fdlg.setWindowTitle("Configure File");
    if (filename_.isEmpty())
        fdlg.setDirectory(".");
    else
        fdlg.setDirectory(filename_);

    //Filter file name
    fdlg.setNameFilter("File(*.xml)");
    fdlg.setFileMode(QFileDialog::ExistingFiles);
    fdlg.setViewMode(QFileDialog::Detail);

    if (fdlg.exec()) {
        QStringList list = fdlg.selectedFiles();
        if (!list.isEmpty()) {
            filename_ = list.takeFirst();
            xml_ = new XmlParse(filename_);
            if (xml_->ParseFile()) {
                CreateWidgetList();
                return;
            }
            delete xml_;
            xml_ = nullptr;
        }
    }
}

void XToolForm::CreateWidgetList()
{
    if (!list_widget_)
        return;

    for (XmlDataNode *node : xml_->root()) {
        if (node) {
            XmlWidgetItem *item = new XmlWidgetItem(node);
            list_widget_->addItem(item);
        }
    }
}

void XToolForm::OnListActived(QListWidgetItem *item)
{
    ShowCurrentItem(item);
}

bool XToolForm::BuildPacket(QListWidgetItem *widget_item,
                            QByteArray *ba,
                            int *timeout)
{
    XmlWidgetItem *item = static_cast<XmlWidgetItem *>(widget_item);
    bool okay;

    stp_->Reset();
    if (timeout)
        *timeout = item->timeout().toInt(&okay, 10);
    for (XmlDataNode::MinorItem *curr : item->minor()) {
        if (curr) {
            int slave = curr->minor.toInt(&okay, 16);
            if (curr->data.isEmpty()) {
                stp_->AppendMessage(slave, nullptr, 0);
            } else {
                QByteArray data = QByteArray::fromHex(curr->data.toLatin1());
                stp_->AppendMessage(slave, data.data(), (quint16)data.length());
            }
        }
    }

    int master = item->major().toInt(&okay, 16);
    return stp_->GeneratePacket(master, 0, ba);
}

void XToolForm::ProvideContextMenu(const QPoint &point)
{
    QListWidgetItem *curr_item = list_widget_->itemAt(point);
    if (!curr_item)
        return;

    QMenu sub_menu;
    sub_menu.addSeparator();
    sub_menu.addAction("Execute");
    sub_menu.addSeparator();
    sub_menu.addAction("BreakPoint");
    QAction *right_click = sub_menu.exec(QCursor::pos());
    if (right_click) {
        if (right_click->text().contains("Execute")) {
            ExecuteCurrentItem(curr_item);
        } else if (right_click->text().contains("BreakPoint")) {
            XmlWidgetItem *item = static_cast<XmlWidgetItem *>(curr_item);
            if (!item->breakpoint()) {
                item->set_breakpoint(true);
                item->setIcon(QIcon(":/images/bpon.png"));
            } else {
                item->set_breakpoint(false);
                item->setIcon(QIcon());
            }
        }
    }
}

void XToolForm::OnReceiveMessage(ByteArrayNode *node)
{
    QByteArray *buf = node->data();
    stp_->ProcessMessage(*buf);
    QByteArray str(buf->toHex(' ').toUpper());
    text_browser_->append("Received: " + str);
    node->Release();
}

void XToolForm::PortChangedStatus(const QString &s)
{
    statusBar()->setStatusTip(s);
}

void XToolForm::OnTimeout()
{
    int curr_row = list_widget_->currentRow() + 1;
    if (curr_row < list_widget_->count()) {
        QListWidgetItem *curr = list_widget_->item(curr_row);
        XmlWidgetItem *item = static_cast<XmlWidgetItem *>(curr);
        list_widget_->setCurrentItem(curr);
        if (!item->breakpoint()) {
            ShowCurrentItem(curr);
            ExecuteCurrentItem(curr);
        }
    }
}

void XToolForm::ExecuteCurrentItem(QListWidgetItem *curr)
{
    QByteArray buffer;
    int timeout;
    if (BuildPacket(curr, &buffer, &timeout)) {
        QString str(buffer.toHex(' ').toUpper());
        text_browser_->append("Packet: " + str);
        this->SendMessage(buffer);
        timer_->setSingleShot(true);
        timer_->start(timeout);
    }
}

void XToolForm::ShowCurrentItem(QListWidgetItem *item)
{
    XmlWidgetItem *xml_widget = static_cast<XmlWidgetItem *>(item);
    text_browser_->clear();
    text_browser_->append("Major:" + xml_widget->major());
    text_browser_->append("Timeout:" + xml_widget->timeout());
    for (XmlDataNode::MinorItem *minor : xml_widget->minor()) {
        if (minor) {
           text_browser_->append("Minor: " + minor->minor);
           if (minor->data.isEmpty()) {
               text_browser_->append("Length: 0");
               text_browser_->append("Data：");
           } else {
               QByteArray data = QByteArray::fromHex(minor->data.toLatin1());
               QString str = QString("Length: %1").arg(data.length());
               text_browser_->append(str);
               text_browser_->append("Data：" + minor->data);
           }
           text_browser_->append("Expect: " + minor->expect);
        }
    }
}


