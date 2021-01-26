#include <QFile>
#include <QComboBox>
#include <QVBoxLayout>
#include <QPushButton>

#include <QSerialPort>
#include <QSerialPortInfo>

#include <QMessageBox>

#include "ui_serialform.h"
#include "serialform.h"
#include "serial.h"
#include "xtoolform.h"

namespace {
const QSerialPort::BaudRate baudrate_table[] = {
    QSerialPort::Baud115200,
    QSerialPort::Baud57600,
    QSerialPort::Baud38400,
    QSerialPort::Baud19200,
    QSerialPort::Baud9600,
    QSerialPort::Baud4800,
    QSerialPort::Baud2400,
    QSerialPort::Baud1200
};

const QSerialPort::DataBits databit_table[] = {
    QSerialPort::Data8,
    QSerialPort::Data7,
    QSerialPort::Data6,
    QSerialPort::Data5
};

const QSerialPort::StopBits stopbit_table[] = {
    QSerialPort::OneStop,
    QSerialPort::TwoStop
};

const QSerialPort::Parity parity_table[] = {
    QSerialPort::NoParity,
    QSerialPort::EvenParity,
    QSerialPort::OddParity
};
}

SerialForm::SerialForm(QWidget *parent)
    : QWidget(parent),
      ui_(new Ui::SerialForm)
{
    ui_->setupUi(this);
    port_ = ui_->comboBoxPort;
    baudrate_ = ui_->comboBoxBaudrate;
    databit_ = ui_->comboBoxDatabit;
    stopbit_ = ui_->comboBoxStopbit;
    parity_ = ui_->comboBoxParity;
    done_ = ui_->pushButtonEnter;

    //Scan avalible serial ports of system
    ScanAvaliblePort();

    //Connect events
    connect(done_, &QPushButton::clicked, this, &SerialForm::OnPushButtonClicked);

    Qt::WindowFlags flags = windowFlags();
    flags &= ~(Qt::WindowMaximizeButtonHint | Qt::WindowMinMaxButtonsHint);
    setWindowFlags(flags);
    setFixedSize(width(), height());
}

SerialForm::~SerialForm()
{
    delete ui_;
}

void SerialForm::ScanAvaliblePort()
{
    for (const QSerialPortInfo &info:
            QSerialPortInfo::availablePorts()) {
        port_->addItem(info.portName());
    }
}

void SerialForm::OnPushButtonClicked()
{
    if (port_->currentIndex() < 0)
        goto _close;

    SerialThread::SerialParam param;
    param.baudrate = baudrate_table[baudrate_->currentIndex()];
    param.data_bits = databit_table[databit_->currentIndex()];
    param.stop_bits = stopbit_table[stopbit_->currentIndex()];
    param.parity = parity_table[parity_->currentIndex()];

    SerialThread *serial;
    if (!master_->serial_) {
        serial = new SerialThread();
        master_->serial_ = serial;
    } else {
        serial = master_->serial_;
    }

    connect(serial, &SerialThread::RecvMessage,
            master_, &XToolForm::OnReceiveMessage);
    connect(serial, &SerialThread::Connected,
            master_, &XToolForm::OnPortChangedStatus);
    connect(serial, &SerialThread::Disconnected,
            master_, &XToolForm::OnPortChangedStatus);
    if (!serial->Start(port_->currentText(), param))
        delete serial;

_close:
    this->close();
}
