#include "serialport_view.h"
#include "serial.h"


SerialPortView::SerialPortView(QWidget *parent)
    : QWidget(parent)
{
    ui_.setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::Popup);
}

void SerialPortView::Update(int action, void *ptr)
{
    if (action == SerialThread::SERIAL_COM_ADD) {
        QString *str = (QString *)ptr;
        if (ui_.comboBoxPort->findText(*str) < 0)
            ui_.comboBoxPort->addItem(*str);
    }
}

void SerialPortView::closeEvent(QCloseEvent *ev)
{
    QWidget::closeEvent(ev);
    serial_->StopPortScan();
    ui_.comboBoxPort->clear();
}
