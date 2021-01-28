#include <QTimer>

#include "transfer/transfer_controller.h"
#include "transfer/transfer_view.h"
#include "transfer/file_transfer.h"

#include "com/serial/serialform.h"
#include "com/serial/serial.h"
#include "xml/xmlparse.h"
#include "xml/xmlwidget.h"
#include "stp/stp.h"

#include "xtool_model.h"

XToolModel::XToolModel(QObject *parent)
    : QObject(parent),
      observer_list_(),
      serial_(nullptr),
      xml_(nullptr),
      stp_(new StpOpcode),
      timer_(new QTimer),
      waiting_ack_(new QByteArray),
      retrans_timer_(new QTimer)
{
    waiting_ack_->resize(1024);
    timer_->setSingleShot(true);
    retrans_timer_->setSingleShot(true);
    ResumeMessageProcess();
}

XToolModel::~XToolModel()
{
    CloseSerialPort();
    if (stp_) {
        delete stp_;
        stp_ = nullptr;
    }
    if (xml_) {
        delete xml_;
        xml_ = nullptr;
    }
    if (waiting_ack_) {
        delete waiting_ack_;
        waiting_ack_ = nullptr;
    }
    if (retrans_timer_) {
        delete retrans_timer_;
        retrans_timer_ = nullptr;
    }
}

bool XToolModel::Open()
{
    XmlParse *xml = new XmlParse(filename_);
    bool okay = xml->ParseFile();
    if (xml_)
        delete xml_;
    xml_= xml;
    return okay;
}

bool XToolModel::Send(const QByteArray &buf)
{
    if (serial_ == nullptr)
        return false;
    return serial_->SendTo(buf);
}

void XToolModel::ResumeMessageProcess()
{
    auto fn = std::bind(&XToolModel::MainMessageProcess,
                            this,
                            std::placeholders::_1);
    AttachMessageHandler(fn);
}

void XToolModel::ExecuteItem(XmlDataNode *item)
{
    QByteArray *buffer = waiting_ack_;
    int timeout;
    buffer->clear();
    if (BuildPacket(item, buffer, &timeout)) {
        NotifyObservers(XTOOL_SENDMSG_SHOW, buffer);
        this->Send(*buffer);
        timer_->start(timeout);
        StartTransmitTimer();
    }
}

void XToolModel::Stop()
{
    if (timer_->isActive())
        timer_->stop();
}

void XToolModel::CloseSerialPort()
{
    SerialThread *serial = serial_;
    if (serial) {
        serial_ = nullptr;
        delete serial;
    }
}

void XToolModel::OnReceiveMessage(ByteArrayNode *node)
{
    process_fn_(node->data());
    node->Release();
}

void XToolModel::OnTimeout()
{
    NotifyObservers(XTOOL_EXECUTE_NEXT, nullptr);
}

void XToolModel::OnRetransmitTimeout()
{
    if (retrans_count_ > 0) {
        retrans_count_--;
        Send(*waiting_ack_);
        retrans_timer_->start(RETRANS_TIMEOUT);
        NotifyObservers(XTOOL_SENDMSG_SHOW, waiting_ack_);
    }
}

bool XToolModel::BuildPacket(XmlDataNode *node, QByteArray *ba, int *timeout)
{
    int master, slave;
    bool okay;

    if (timeout)
        *timeout = node->timeout().toInt(&okay, 10);
    stp_->Reset();
    for (XmlDataNode::MinorItem *curr : node->minor()) {
        if (curr == nullptr)
            continue;
        slave = curr->minor.toInt(&okay, 16);
        if (curr->data.isEmpty()) {
            stp_->AppendMessage(slave, nullptr, 0);
        } else {
            QByteArray buffer = QByteArray::fromHex(curr->data.toLatin1());
            stp_->AppendMessage(slave, buffer.data(), (quint16)buffer.length());
        }
    }
    master = node->major().toInt(&okay, 16);
    return stp_->GeneratePacket(master, 0, ba);
}

void XToolModel::StartTransmitTimer()
{
    retrans_count_ = RETRANS_COUNT;
    retrans_timer_->start(RETRANS_TIMEOUT);
}

void XToolModel::StopTransmitTimer()
{
    if (retrans_timer_->isActive())
        retrans_timer_->stop();
}

bool XToolModel::MainMessageProcess(QByteArray *buf)
{
    StopTransmitTimer();
    NotifyObservers(XTOOL_RECVMSG_SHOW, buf);
    if (xml_->empty())
        return false;

    if (stp_->ProcessMessage(*buf)) {
        NotifyObservers(XTOOL_EXECUTE_NEXT, nullptr);
        return true;
    }
    return false;
}
