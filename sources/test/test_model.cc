#include <QString>
#include <QDateTime>
#include <QByteArray>
#include <QTimer>

#include "test_model.h"
#include "com/serial/serial.h"
#include "stp/stp.h"
#include "xtool_model.h"

#include "protobuf/remind.pb.h"
#include "protobuf/devinfo.pb.h"

TestModel::TestModel(XToolModel *model, QObject *parent)
    : QObject(parent),
      stp_(new StpOpcode),
      txbuf_(new QByteArray),
      timer_(new QTimer),
      xmodel_(model),
      recv_fn_(nullptr)
{
    txbuf_->resize(512);
    timer_->setSingleShot(true);
    connect(timer_, &QTimer::timeout, this, &TestModel::OnTimeout);
    auto fn = std::bind(&TestModel::ReceiveProcess, this, std::placeholders::_1);
    model->AttachMessageHandler(fn);
}

TestModel::~TestModel()
{
    xmodel_->ResumeMessageProcess();
    delete stp_;
    delete txbuf_;
    delete timer_;
}

bool TestModel::StartCalling(const QString &name, const QString &phone)
{
    QDateTime time = QDateTime::currentDateTime();
    remind::Call calling;
    char buffer[256];

    uint timestamp = time.toTime_t();
    calling.set_timestamp(timestamp);
    calling.set_phone(phone.toStdString());
    calling.set_people(name.toStdString());
    calling.SerializeToArray(buffer, calling.ByteSizeLong());
    SetReceiveProcess(nullptr);
    return SendPacket(STP_REMIND_CLASS, 0x02, buffer, (quint16)calling.ByteSizeLong());
}

bool TestModel::SendTextMessage(const QString &name, const QString &phone, int type,
                                const QString &text)
{
    QDateTime time = QDateTime::currentDateTime();
    remind::Message message;
    char buffer[256];

    uint timestamp = time.toTime_t();
    message.set_timestamp(timestamp);
    message.set_phone(phone.toStdString());
    message.set_people(name.toStdString());
    message.set_type((remind::Message::Type)type);
    message.set_text(text.toStdString());
    message.SerializeToArray(buffer, message.ByteSizeLong());
    SetReceiveProcess(nullptr);
    return SendPacket(STP_REMIND_CLASS, 0x01, buffer, (quint16)message.ByteSizeLong());
}

bool TestModel::ReadDeviceInformation(void)
{
    SetReceiveProcess(&TestModel::UnpackDeviceInformation);
    return SendPacket(STP_INFO_CLASS, 0x01, nullptr, 0);
}

bool TestModel::SendPacket(int major, int minor, const char *buffer, quint16 len)
{
    bool okay = false;
    if (xmodel_->serial()) {
        txbuf_->clear();
        stp_->Reset();
        stp_->AppendMessage(minor, buffer, len);
        stp_->GeneratePacket(major, 0, txbuf_);
        major_ = major;
        minor_ = minor;
        retry_ = 2;
        Notify(SEND_PACKET, txbuf_);
        timer_->start(1000);
        okay = xmodel_->Send(*txbuf_);
    } else {
        QString str("Error: please open a valid serial port\n");
        Notify(SEND_FAILED, &str);
    }
    return okay;
}

bool TestModel::ReceiveProcess(QByteArray *buf)
{
    if (timer_->isActive())
        timer_->stop();

    QString str;
    Notify(RECV_PACKET, buf);
    if (stp_->ProcessMessage(*buf)) {
        quint16 len;
        const quint8 *packet = stp_->ToL2(*buf, &len);
        if (packet[0] != major_) {
            str = "Error: major number is not matched!";
            goto _failed;
        }
        if (packet[1] != minor_) {
            str = "Error: minor number is not matched!";
            goto _failed;
        }
        if (len < 3 || (len == 3 && packet[2] != 0)) {
            str = "Error: packet length is not matched or result error!";
            goto _failed;
        }
        if (len >= 4) {
            if (recv_fn_) {
                quint16 data_len = Netbuffer::ByteToCpu16(&packet[2]);
                if (!(this->*recv_fn_)(&packet[4], data_len)) {
                    str = "Error: data packet is invalid\n";
                    goto _failed;
                }
            }
        }
        str = "Send successful!";
        Notify(SEND_COMPLETED, &str);
        return true;
    }

_failed:
    Notify(SEND_FAILED, &str);
    return false;
}

bool TestModel::UnpackDeviceInformation(const quint8 *packet, quint16 len)
{
    info::Device dinfo;
    bool result;
    result = dinfo.ParseFromArray(packet, len);
    if (result)
        Notify(DEVINFO_PACKET, &dinfo);
    return result;
}

void TestModel::OnTimeout()
{
    if (retry_ > 0) {
        xmodel_->Send(*txbuf_);
        if (--retry_ > 0) {
            timer_->start(1000);
            return;
        }
    }
    QString str("Error: device is no respond!");
    Notify(SEND_FAILED, &str);
}
