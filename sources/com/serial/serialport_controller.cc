#include "serialport_controller.h"
#include "serialport_view.h"
#include "serial.h"
#include "ui_serialform.h"

#include "xtool_controller.h"
#include "xtool_model.h"

SerialPortController::SerialPortController(SerialPortView *view,
                                           SerialThread *serial,
                                           QObject *parent)
    : QObject(parent), view_(view), serial_(serial)
{
    //Connect events
    connect(view_->ui_.pushButtonEnter, &QPushButton::clicked,
            this, &SerialPortController::OnPushButtonEnter);
    view_->SetSerialThread(serial_);
    serial_->AddObserver(view_);
}

SerialPortController::~SerialPortController()
{
    if (serial_) {
        delete serial_;
        serial_ = nullptr;
    }
}

void SerialPortController::Show()
{
    view_->showNormal();
    serial_->StartPortScan();
}

void SerialPortController::OnPushButtonEnter(void)
{
    if (view_->port_index() < 0) {
        view_->close();
        return;
    }

    serial_->SetBaudrate(view_->baudrate_index());
    serial_->SetDatabit(view_->databit_index());
    serial_->SetStopbit(view_->stopbit_index());
    serial_->SetParity(view_->parity_index());
    serial_->Start(view_->portname());
    view_->close();
}
