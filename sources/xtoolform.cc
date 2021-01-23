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
#include <QCheckBox>
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
      timer_(new QTimer()),
      waiting_ack_(new QByteArray()),
      retrans_timer_(new QTimer())
{
    QUiLoader loader;

    waiting_ack_->resize(1024);
    timer_->setSingleShot(true);
    retrans_timer_->setSingleShot(true);

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
    item_text_browser_ = findChild<QTextBrowser *>("textBrowserItem");

    start_btn_ = findChild<QPushButton *>("pushButtonStart");
    stop_btn_ = findChild<QPushButton *>("pushButtonStop");
    stop_btn_->setEnabled(false);

    skip_chkbox_ = findChild<QCheckBox *>("checkBoxSkipBox");

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
    connect(list_widget_, &QListWidget::currentItemChanged,
            this, &XToolForm::OnExecuteItemChanged);
    connect(start_btn_, &QPushButton::clicked,
            this, &XToolForm::OnStartExecute);
    connect(stop_btn_, &QPushButton::clicked,
            this, &XToolForm::OnStopExecute);

    //Set right-click menu for list-widget item
    list_widget_->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(list_widget_, &QListWidget::customContextMenuRequested,
            this, &XToolForm::ProvideContextMenu);

    //Timer
    connect(timer_, &QTimer::timeout,
            this, &XToolForm::OnTimeout);
    connect(retrans_timer_, &QTimer::timeout,
            this, &XToolForm::OnRetransmitTimeout);

    widget->setWindowFlags(Qt::SubWindow);
    setWindowTitle(tr("xtool"));
    setWindowIcon(QIcon(":/images/main_icon.png"));
    //setCentralWidget(widget);

    ResumeMessageProcess();
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
    TransferForm *form = new TransferForm(this);
    form->showNormal();
    process_fn_ = std::bind(&TransferForm::FileMessageProcess,
                            form,
                            std::placeholders::_1);
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
            item->setIcon(QIcon(":/images/bpoff.png"));
            list_widget_->addItem(item);
        }
    }
}

void XToolForm::StartExecute(QListWidgetItem *curr, bool walk_around)
{
    QBrush curr_brush(QColor(255, 0, 255, 127));
    curr->setBackground(curr_brush);
    start_btn_->setEnabled(false);
    stop_btn_->setEnabled(true);
    ExecuteCurrentItem(curr, walk_around);
}

void XToolForm::StopExecute(QListWidgetItem *curr)
{
    if (timer_->isActive())
        timer_->stop();

    QBrush brush(QColor(255, 255, 255));
    curr->setBackground(brush);
    stop_btn_->setEnabled(false);
    start_btn_->setEnabled(true);
    GenerateResult();
}

void XToolForm::OnStartExecute()
{
    StartExecute(list_widget_->currentItem(), true);
}

void XToolForm::OnStopExecute()
{
    StopExecute(list_widget_->currentItem());
}

void XToolForm::OnListActived(QListWidgetItem *item)
{
    ShowCurrentItem(item, item_text_browser_, true);
}

void XToolForm::OnExecuteItemChanged(QListWidgetItem *curr,
                                     QListWidgetItem *prev)
{
    if (stop_btn_->isEnabled()) {
        if (prev) {
            QBrush prev_brush(QColor(255, 255, 255));
            prev->setBackground(prev_brush);
        }
        if (curr) {
            QBrush curr_brush(QColor(255, 0, 255, 127));
            curr->setBackground(curr_brush);
        }
    }
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
    QAction *exec_action = sub_menu.addAction("Execute");
    sub_menu.addSeparator();
    QAction *bkpt_action =sub_menu.addAction("BreakPoint");
    QAction *right_click = sub_menu.exec(QCursor::pos());
    if (right_click) {
        if (right_click == exec_action) {
            StartExecute(curr_item, true);
        } else if (right_click == bkpt_action) {
            XmlWidgetItem *item = static_cast<XmlWidgetItem *>(curr_item);
            if (!item->breakpoint()) {
                item->set_breakpoint(true);
                item->setIcon(QIcon(":/images/bpon.png"));
            } else {
                item->set_breakpoint(false);
                item->setIcon(QIcon(":/images/bpoff.png"));
            }
        }
    }
}

void XToolForm::MainMessageProcess(QByteArray *buf)
{
    StopTransmitTimer();
    QByteArray str(buf->toHex(' ').toUpper());
    text_browser_->append("Received: " + str);

    if (!list_widget_->count())
        return;
    bool okay = stp_->ProcessMessage(*buf);
    XmlWidgetItem *item = static_cast<XmlWidgetItem *>(list_widget_->currentItem());
    item->set_result(okay);
    if (okay || skip_chkbox_->isChecked()) {
        if (!ExecuteNextItem()) {
            StopExecute(list_widget_->currentItem());
        }
    } else {
        StopExecute(list_widget_->currentItem());
    }
}

void XToolForm::OnReceiveMessage(ByteArrayNode *node)
{
    process_fn_(node->data());
    node->Release();
}

void XToolForm::PortChangedStatus(const QString &s)
{
    statusBar()->setStatusTip(s);
}

bool XToolForm::ExecuteNextItem(void)
{
    int next_row = list_widget_->currentRow() + 1;
    if (next_row < list_widget_->count()) {
        QListWidgetItem *next = list_widget_->item(next_row);
        XmlWidgetItem *item = static_cast<XmlWidgetItem *>(next);
        list_widget_->setCurrentItem(next);
        if (item->breakpoint())
            return false;

        ExecuteCurrentItem(next, true);
        return true;
    }
    return false;
}

void XToolForm::OnTimeout()
{
    QListWidgetItem *curr = list_widget_->currentItem();
    MarkingErrorItem(curr);
    if (skip_chkbox_->isChecked()) {
        if (!ExecuteNextItem())
            StopExecute(curr);
    } else {
        StopExecute(curr);
    }
}

void XToolForm::ExecuteCurrentItem(QListWidgetItem *curr, bool walk_around)
{
    QByteArray *buffer = waiting_ack_;
    int timeout;

    buffer->clear();
    ShowCurrentItem(curr, text_browser_, false);
    if (BuildPacket(curr, buffer, &timeout)) {
        QString str(buffer->toHex(' ').toUpper());
        text_browser_->append("Packet: " + str);
        this->SendMessage(*buffer);
        if (walk_around)
            timer_->start(timeout);
        StartTransmitTimer();
    }
}

void XToolForm::ShowCurrentItem(QListWidgetItem *item, QTextBrowser *text, bool clear)
{
    XmlWidgetItem *xml = static_cast<XmlWidgetItem *>(item);
    if (clear)
        text->clear();

    AddInformationHeader(text, xml->name());
    //text->append(xml->name());
    text->append("Major:" + xml->major());
    text->append("Timeout:" + xml->timeout());
    for (XmlDataNode::MinorItem *minor : xml->minor()) {
        if (minor) {
           text->append("Minor: " + minor->minor);
           if (minor->data.isEmpty()) {
               text->append("Length: 0");
               text->append("Data：");
           } else {
               QByteArray data = QByteArray::fromHex(minor->data.toLatin1());
               QString str = QString("Length: %1").arg(data.length());
               text->append(str);
               text->append("Data：" + minor->data);
           }
           text->append("Expect: " + minor->expect);
        }
    }
}

void XToolForm::MarkingErrorItem(QListWidgetItem *item)
{
    XmlWidgetItem *xml_item = static_cast<XmlWidgetItem *>(item);
    QBrush brush(QColor(255, 0, 0, 127));
    item->setBackground(brush);
    xml_item->set_result(false);
}

void XToolForm::OnRetransmitTimeout()
{
    if (retrans_count_ > 0) {
        retrans_count_--;
        SendMessage(*waiting_ack_);
        retrans_timer_->start(RETRANS_TIMEOUT);
        QString str(waiting_ack_->toHex(' ').toUpper());
        text_browser_->append("Retransmit packet: " + str);
    }
}

void XToolForm::ResumeMessageProcess()
{
    process_fn_ = std::bind(&XToolForm::MainMessageProcess,
                            this,
                            std::placeholders::_1);
}

void XToolForm::StartTransmitTimer()
{
    retrans_count_ = RETRANS_COUNT;
    retrans_timer_->start(RETRANS_TIMEOUT);
}

void XToolForm::StopTransmitTimer()
{
    if (retrans_timer_->isActive())
        retrans_timer_->stop();
}

void XToolForm::GenerateResult()
{
    int i, count=0;

    text_browser_->append("\n******** Statistics Information ********");
    for (i = 0; i <= list_widget_->currentRow(); i++) {
        QListWidgetItem *curr = list_widget_->item(i);
        XmlWidgetItem *item = static_cast<XmlWidgetItem *>(curr);
        if (!item->result() && !item->breakpoint()) {
            QString str("Error: ");
            str.append(item->name());
            text_browser_->append(str);
            count++;
        }
    }
    if (count) {
        QString str = QString("Total: %1, Total errors: %2").arg(i).arg(count);
        text_browser_->append(str);
    } else {
        QString str = QString("Total: %1").arg(count);
        text_browser_->append("Successful!");
    }
}

void XToolForm::AddInformationHeader(QTextBrowser *text, const QString &info)
{
   text->append("=============================");
   text->append(info);
   text->append("=============================");
}
