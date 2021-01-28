#ifndef SERIALPORT_VIEW_H_
#define SERIALPORT_VIEW_H_

#include <QWidget>

#include "observer.h"
#include "ui_serialform.h"

namespace Ui {
class SerialForm;
}
class XToolModel;
class XToolController;
class SerialPortController;
class SerialThread;

class SerialPortView : public QWidget, public ObserverBase {
    Q_OBJECT

public:
    friend class SerialPortController;
    explicit SerialPortView(QWidget *parent = nullptr);
    ~SerialPortView() {}

    void Update(int action, void *ptr) override;
    void closeEvent(QCloseEvent *ev) override;

    void SetSerialThread(SerialThread *serial) {
        serial_ = serial;
    }
    int baudrate_index() const {
        return ui_.comboBoxBaudrate->currentIndex();
    }
    int databit_index() const {
        return ui_.comboBoxDatabit->currentIndex();
    }
    int stopbit_index() const {
        return ui_.comboBoxStopbit->currentIndex();
    }
    int parity_index() const {
        return ui_.comboBoxParity->currentIndex();
    }
    int port_index() const {
        return ui_.comboBoxPort->currentIndex();
    }
    QString portname() const {
        return ui_.comboBoxPort->currentText();
    }

private:
    Ui::SerialForm ui_;
    SerialThread *serial_;
};

#endif // SERIALPORT_VIEW_H_
