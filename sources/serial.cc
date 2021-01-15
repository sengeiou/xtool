#include <QSerialPort>
#include <QMutex>

#include "serial.h"


SerialThread::SerialThread(QObject *parent)
    : QThread(parent), actived_(0), port_updated_(0),
      list_mutex_(new QMutex), send_timeout_(3000)
{
    tx_queue_ = new ByteArrayList();
    node_pool_ = new ByteArrayNodePool(8, 1024);
}

SerialThread::~SerialThread()
{
    Stop();
    if (list_mutex_)
        delete list_mutex_;

    delete tx_queue_;
    delete node_pool_;
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
    ByteArrayNode *node = AllocateNode(true);
    node->data()->append(data);
    list_mutex_->lock();
    tx_queue_->Append(node);
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
    serial.setPortName(curr_port);
    if (serial.isOpen()) {
        serial.clear();
        serial.close();
    }

    serial.setBaudRate(param_.baudrate);
    serial.setDataBits(param_.data_bits);
    serial.setStopBits(param_.stop_bits);
    serial.setParity(param_.parity);
    if (!serial.open(QIODevice::ReadWrite)) {
        QString info(port_name_);
        emit this->Error(info + " is busy");
        return;
    }

    emit this->Connected("Connected");
    while (actived_.load()) {
        list_mutex_->lock();
        if (!tx_queue_->empty()) {
            ByteArrayNode *tx_node = tx_queue_->TakeFirst();
            list_mutex_->unlock();
            serial.write(*(tx_node->data()));
            if (!serial.waitForBytesWritten(send_timeout_))
                emit this->Error("Send timeout");
            tx_node->Release();
        } else {
            list_mutex_->unlock();
        }

        if (serial.waitForReadyRead(10)) {
            ByteArrayNode *rx_node = AllocateNode(false);
            QByteArray *rx_buf = rx_node->data();
            if (rx_buf) {
                *rx_buf = serial.readAll();
                while (serial.waitForReadyRead(10))
                    (*rx_buf) += serial.readAll();
                emit this->RecvMessage(rx_node);
            } else {
                //Drop messages if buffer is no enought
                serial.readAll();
            }
        }
        if (port_updated_.load())
            goto _reinit;
    }

    serial.close();
    emit this->Disconnected("Disconnected");
}

