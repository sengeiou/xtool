#ifndef SERIALPORT_CONTROLLER_H_
#define SERIALPORT_CONTROLLER_H_

#include <QObject>

class SerialPortView;
class SerialThread;
class XToolModel;
class QByteArray;

class SerialPortController : public QObject {
    Q_OBJECT
public:
    explicit SerialPortController(SerialPortView *view,
                                  SerialThread *serial,
                                  QObject *parent = nullptr);
    ~SerialPortController();
    void Show();

private slots:
    void OnPushButtonEnter(void);

private:
    SerialPortView *view_;
    SerialThread *serial_;
};

#endif // SERIALPORT_CONTROLLER_H_
