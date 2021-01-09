#include <QSerialPort>
#include <QMutex>

#include "serial.h"


SerialThread::SerialThread(QObject *parent)
    : QThread(parent), actived_(0), port_updated_(0),
      list_mutex_(new QMutex), send_timeout_(3000)
{

}

SerialThread::~SerialThread()
{
    Stop();
    if (list_mutex_)
        delete list_mutex_;
}

bool SerialThread::Start(const QString &portname,
                         const SerialParam &param)
{
    port_name_ = portname;
    param_ = param;
    if (!actived_.load()) {
        actived_.store(1);
        start();
    } else {
        RequestUpdatePort();
    }
    return true;
}

void SerialThread::Stop()
{
    if (actived_.load()) {
        actived_.store(0);
        wait();
    }
}

bool SerialThread::SendTo(const QByteArray &data)
{
    list_mutex_->lock();
    tx_list_.append(data);
    list_mutex_->unlock();
    return true;
}

void SerialThread::run()
{
    QString curr_port(port_name_);
    QSerialPort serial;

_reinit:
    port_updated_.store(0);
    if (curr_port != port_name_) {
        serial.close();
        curr_port = port_name_;
    }

    //Set serial port name and check status
    serial.setPortName(curr_port);
    if (serial.isOpen()) {
        serial.clear();
        serial.close();
    }

    //Set serial port format
    serial.setBaudRate(param_.baudrate);
    serial.setDataBits(param_.data_bits);
    serial.setStopBits(param_.stop_bits);
    serial.setParity(param_.parity);
    if (!serial.open(QIODevice::ReadWrite)) {
        QString info(port_name_);
        emit this->Error(info + " is busy");
        return;
    }

    while (actived_.load()) {
        list_mutex_->lock();
        if (!tx_list_.isEmpty()) {
            const QByteArray &tx_data = tx_list_.takeFirst();
            list_mutex_->unlock();

            serial.write(tx_data);
            if (!serial.waitForBytesWritten(send_timeout_))
                emit this->Error("Send timeout");
        } else {
            list_mutex_->unlock();
        }

        if (serial.waitForReadyRead(10)) {
            QByteArray recv_data = serial.readAll();
            while (serial.waitForReadyRead(10))
                recv_data += serial.readAll();
            emit this->Received(recv_data);
        }

        if (port_updated_.load())
            goto _reinit;
    }

    serial.close();
}

