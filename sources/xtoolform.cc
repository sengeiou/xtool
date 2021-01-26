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
#include <QSpacerItem>
#include <QByteArray>
#include <QTimer>

#include "transfer/transfer_controller.h"
#include "transfer/transfer_view.h"
#include "transfer/file_transfer.h"

#include "ui_xtoolform.h"
#include "xtoolform.h"

#include "serialform.h"
#include "xmlparse.h"
#include "xmlwidget.h"
#include "stp.h"
#include "serial.h"




XToolForm::XToolForm(QWidget *parent)
    : QMainWindow(parent),
      ui_(new Ui::MainWindow),
      transfer_controller_(nullptr),
      serial_(nullptr),
      serial_form_(nullptr),
      xml_(nullptr),
      stp_(new StpOpcode),
      timer_(new QTimer),
      waiting_ack_(new QByteArray),
      retrans_timer_(new QTimer)
{
    ui_->setupUi(this);

    waiting_ack_->resize(1024);
    timer_->setSingleShot(true);
    retrans_timer_->setSingleShot(true);

    //File menu
    open_ = ui_->actionOpen;
    exit_ = ui_->actionExit;

    //Port menu
    conn_ = ui_->actionConnect;

    //About
    about_ = ui_->actionAbout;

    //File transmit group
    transfer_ = ui_->actionSend_File;

    list_widget_ = ui_->listWidget;
    text_browser_ = ui_->textBrowser;
    item_text_browser_ = ui_->textBrowserItem;

    start_btn_ = ui_->pushButtonStart;
    stop_btn_ = ui_->pushButtonStop;
    stop_btn_->setEnabled(false);

    skip_chkbox_ = ui_->checkBoxSkipBox;

    //Toolbar
    QToolBar *toolbar = ui_->toolBar;
    toolbar_conn_ = toolbar->addAction(QIcon(":/images/connect.png"),
                                       "Connect");
    toolbar_disconn_ = toolbar->addAction(QIcon(":/images/disconnect.png"),
                                       "Disconnect");
    toolbar_setting_ = toolbar->addAction(QIcon(":/images/settings.png"),
                                          "Setting");
    toolbar_clear_ = toolbar->addAction(QIcon(":/images/clear.png"),
                                       "Clear");
    toolbar_disconn_->setEnabled(false);
    toolbar_conn_->setEnabled(true);

    //StatusBar
    QStatusBar *statusbar = ui_->statusBar;
    status_label_ = new QLabel("Disconnected");
    statusbar->addPermanentWidget (status_label_);

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

    connect(toolbar_conn_, &QAction::triggered,
            this, &XToolForm::OnActionConnect);
    connect(toolbar_disconn_, &QAction::triggered,
            this, &XToolForm::OnClosePort);
    connect(toolbar_clear_, &QAction::triggered,
            this, &XToolForm::OnClearText);

    //Set right-click menu for list-widget item
    list_widget_->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(list_widget_, &QListWidget::customContextMenuRequested,
            this, &XToolForm::ProvideContextMenu);

    //Timer
    connect(timer_, &QTimer::timeout,
            this, &XToolForm::OnTimeout);
    connect(retrans_timer_, &QTimer::timeout,
            this, &XToolForm::OnRetransmitTimeout);

    setWindowIcon(QIcon(":/images/main_icon.png"));
    //setAttribute(Qt::WA_DeleteOnClose);
    ResumeMessageProcess();
}

XToolForm::~XToolForm()
{
    OnClosePort();
    if (xml_) {
        XmlParse *xml = xml_;
        xml_ = nullptr;
        delete xml;
    }
    if (ui_) {
        Ui::MainWindow *ui = ui_;
        ui_ = nullptr;
        delete ui;
    }
    if (stp_) {
        StpOpcode *stp = stp_;
        stp_ = nullptr;
        delete stp;
    }
}

bool XToolForm::Send(const QByteArray &buf)
{
    if (serial_)
        return serial_->SendTo(buf);
    return false;
}

void XToolForm::OnClosePort()
{
    if (serial_) {
        toolbar_disconn_->setEnabled(false);
        delete serial_;
        serial_ = nullptr;
        toolbar_conn_->setEnabled(true);
    }
}

void XToolForm::OnActionConnect()
{
    SerialForm *form = serial_form_;
    if (!form) {
        form = new SerialForm(this);
        serial_form_ = form;
        form->SetMasterForm(this);
        form->setWindowFlags(Qt::Dialog | Qt::Popup);
    }

    form->showNormal();
}

void XToolForm::OnActionTransfer()
{
    TransferController *controller = transfer_controller_;
    if (controller == nullptr) {
        TransferView *view = new TransferView(this, ui_->textBrowser);
        FileTransfer *transfer = new FileTransfer(this);
        controller = new TransferController(view, transfer);
        transfer_controller_ = controller;
    }
    controller->Show();
    process_fn_ = std::bind(&TransferController::MessageProcess,
                            controller,
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
                setWindowTitle(QString("xtool [%1]").arg(filename_));
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

void XToolForm::OnPortChangedStatus(const QString &s, bool open)
{
    if (open) {
        toolbar_conn_->setEnabled(false);
        toolbar_disconn_->setEnabled(true);
    } else {
        toolbar_conn_->setEnabled(true);
        toolbar_disconn_->setEnabled(false);
    }
    status_label_->setText(s);
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
        this->Send(*buffer);
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
        Send(*waiting_ack_);
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

void XToolForm::OnClearText()
{
    text_browser_->clear();
}
