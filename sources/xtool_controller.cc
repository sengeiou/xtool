#include <QString>
#include <QByteArray>

#include "transfer/transfer_controller.h"
#include "transfer/transfer_view.h"
#include "transfer/file_transfer.h"

#include "xtool_controller.h"
#include "xtool_view.h"
#include "xtool_model.h"

#include "com/serial/serial.h"
#include "com/serial/serialport_controller.h"
#include "com/serial/serialport_view.h"

#include "xml/xmlparse.h"
#include "xml/xmlwidget.h"


XToolController::XToolController(XToolView *view, XToolModel *model,
                                 QObject *parent)
    : QObject(parent), ObserverBase(), view_(view), model_(model),
      serial_controller_(nullptr), transfer_controller_(nullptr)
{
    //Connect events
    connect(view_->exit_, &QAction::triggered,
            view_, &QMainWindow::close);
    connect(view_->conn_, &QAction::triggered,
            this, &XToolController::OnOpenSerialPort);
    connect(view_->transfer_, &QAction::triggered,
            this, &XToolController::OnOPenFileTransfer);
    connect(view_->open_, &QAction::triggered,
            this, &XToolController::OnOpenProjectFile);

    connect(view_->list_widget_, &QListWidget::itemClicked,
            this, &XToolController::OnListItemActived);
    connect(view_->start_btn_, &QPushButton::clicked,
            this, &XToolController::OnStartExecute);
    connect(view_->stop_btn_, &QPushButton::clicked,
            this, &XToolController::OnStopExecute);

    connect(view_->toolbar_open_, &QAction::triggered,
            this, &XToolController::OnOpenProjectFile);
    connect(view_->toolbar_conn_, &QAction::triggered,
            this, &XToolController::OnOpenSerialPort);
    connect(view_->toolbar_disconn_, &QAction::triggered,
            this, &XToolController::OnCloseSerialPort);
    connect(view_->toolbar_clear_, &QAction::triggered,
            view_, &XToolView::ClearLogText);
    connect(view_->list_widget_, &QListWidget::customContextMenuRequested,
            this, &XToolController::OnListContextMenu);

    model_->AddObserver(this);
}

XToolController::~XToolController()
{

}

void XToolController::Update(int action, void *ptr)
{
    switch (action) {
    case XToolModel::XTOOL_EXECUTE_NEXT:
        ExecuteNextItem();
        break;
    case XToolModel::XTOOL_EXECUTE_TIMEOUT:
        if (view_->skip_chkbox_->isChecked())
            ExecuteNextItem();
        else
            GenerateStatisticsResult(view_->text_browser_);
        break;
    case XToolModel::XTOOL_RECVMSG_SHOW: {
        QByteArray *buf = (QByteArray *)ptr;
        QByteArray str(buf->toHex(' ').toUpper());
        view_->text_browser_->append("Received: " + str);
        } break;

    case XToolModel::XTOOL_SENDMSG_SHOW: {
        QByteArray *buf = (QByteArray *)ptr;
        QString str(buf->toHex(' ').toUpper());
        view_->text_browser_->append("Retransmit packet: " + str);
        } break;
    default:
        break;
    }
}

void XToolController::Show()
{
    view_->show();
}

void XToolController::GenerateDataList(const QList<XmlDataNode *> &root)
{
    view_->ClearListWidget();
    for (XmlDataNode *node : root) {
        if (node) 
            view_->CreateListWidget(new XmlWidgetItem(node));
    }
}

void XToolController::ExecuteItem(XmlWidgetItem *item)
{
    ShowItem(item, view_->text_browser_, false);
    model_->ExecuteItem(item->data());
}

void XToolController::ExecuteNextItem()
{
    XmlWidgetItem *item = (XmlWidgetItem *)view_->next();
    if (item == nullptr) {
        OnStopExecute();
        GenerateStatisticsResult(view_->text_browser_);
        return;
    }
    if (item->breakpoint()) {
        OnStopExecute();
        GenerateStatisticsResult(view_->text_browser_);
        return;
    }

    ExecuteItem(item);
}

void XToolController::ShowItem(QListWidgetItem *item, QTextBrowser *text,
                               bool clear)
{
    XmlWidgetItem *xml = static_cast<XmlWidgetItem *>(item);
    if (clear)
        text->clear();
    AddInformationHeader(text, xml->name());
    text->append(xml->name());
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

void XToolController::AddInformationHeader(QTextBrowser *text, const QString &info)
{
   text->append("=============================");
   text->append(info);
   text->append("=============================");
}

void XToolController::GenerateStatisticsResult(QTextBrowser *text_browser)
{
    QColor old = text_browser->textColor();
    int i, count=0;
    text_browser->append("\n******** Statistics Information ********");
    text_browser->setTextColor(QColor(255, 0, 0));
    for (i = 0; i <= view_->list_widget_->currentRow(); i++) {
        XmlWidgetItem *item = (XmlWidgetItem *)view_->list_widget_->item(i);
        if (!item->result() && !item->breakpoint()) {
            QString str("Error: ");
            str.append(item->name());
            text_browser->append(str);
            count++;
        }
    }
    if (count) {
        QString str = QString("Total: %1, Total errors: %2").arg(i).arg(count);
        text_browser->append(str);
    } else {

        QString str = QString("Total: %1").arg(count);
        text_browser->setTextColor(QColor(0, 255, 0));
        text_browser->append("Successful!");
    }

    text_browser->setTextColor(old);
}

void XToolController::OnOpenProjectFile()
{
    QString *filename = model_->filename();
    bool okay = view_->OpenFileSystem(filename, "File(*.xml)");
    if (okay) {
        view_->setWindowTitle(QString("xtool [%1]").arg(*filename));
        if (model_->Open())
            GenerateDataList(model_->filenode());
    }
}

void XToolController::OnOpenSerialPort()
{
    SerialPortController *controller = serial_controller_;
    if (controller == nullptr) {
        SerialPortView *view = new SerialPortView(view_);
        SerialThread *serial = new SerialThread();
        controller = new SerialPortController(view, serial);
        model_->SetSerialPort(serial);
        serial_controller_ = controller;

        connect(serial, &SerialThread::Connected,
                this, &XToolController::OnPortChangedStatus);
        connect(serial, &SerialThread::Disconnected,
                this, &XToolController::OnPortChangedStatus);
    }
    controller->Show();
}

void XToolController::OnOPenFileTransfer()
{
    TransferController *controller = transfer_controller_;
    if (controller == nullptr) {
        TransferView *view = new TransferView(view_, view_->ui_->textBrowser);
        FileTransfer *transfer = new FileTransfer(model_);
        controller = new TransferController(view, transfer);
        transfer_controller_ = controller;
    }
    auto fn = std::bind(&TransferController::MessageProcess,
                        controller, std::placeholders::_1);
    model_->AttachMessageHandler(fn);
    controller->Show();
}

void XToolController::OnCloseSerialPort()
{
    model_->CloseSerialPort();
    view_->SetSerialPortStatus(false);
}

void XToolController::OnListItemActived(QListWidgetItem *item)
{
    ShowItem(item, view_->item_text_browser_, true);
}

void XToolController::OnStartExecute()
{
    XmlWidgetItem *item = (XmlWidgetItem *)view_->current();
    ExecuteItem(item);
    view_->StartListProcess(item);
}

void XToolController::OnStopExecute()
{
    model_->Stop();
    view_->StopListProcess(view_->current());
}

void XToolController::OnPortChangedStatus(const QString &s, bool opened)
{
    view_->SetSerialPortStatus(opened);
    view_->SetStatusBarText(s);
}

void XToolController::OnListContextMenu(const QPoint &point)
{
    XmlWidgetItem *item = (XmlWidgetItem *)view_->item_at(point);
    if (item != nullptr) {
        QMenu sub_menu;
        QAction *exec_action = sub_menu.addAction("Execute");
        sub_menu.addSeparator();
        QAction *bkpt_action =sub_menu.addAction("BreakPoint");
        QAction *right_click = sub_menu.exec(QCursor::pos());
        if (right_click) {
            if (right_click == exec_action) {
                OnStartExecute();
            } else if (right_click == bkpt_action) {
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
}

