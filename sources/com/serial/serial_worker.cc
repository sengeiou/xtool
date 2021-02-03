#include <QSerialPort>

#include "serial_worker.h"

SerialWorker::SerialWorker()
    : QObject(parent)
{

}

void SerialWorker::StartWork()
{
    com_ = new QSerialPort();
    com_->open(QIODevice::ReadWrite);
}

void SerialWorker::StopWork()
{
    com_->close();
    delete com_;
    com_ = nullptr;
}
