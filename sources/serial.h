#ifndef SERIAL_H_
#define SERIAL_H_

#include <QThread>
#include <QAtomicInt>
#include <QByteArray>
#include <QList>

#include <QSerialPort>

//class QSerialPort;
class QMutex;


class SerialThread : public QThread {
    Q_OBJECT

public:
    struct SerialParam {
        QSerialPort::BaudRate baudrate;
        QSerialPort::DataBits data_bits;
        QSerialPort::StopBits stop_bits;
        QSerialPort::Parity parity;
    };

    explicit SerialThread(QObject *parent = nullptr);
    ~SerialThread();
    bool Start(const QString &portname, const struct SerialParam &param);
    void Stop();
    bool SendTo(const QByteArray &);
    const QString &port_name() const {
        return port_name_;
    }

signals:
    void Error(const QString &);
    void Received(const QByteArray &);

protected:
    virtual void run() override;

private:
    void RequestUpdatePort() {
        port_updated_.store(1);
    }
private:
    QAtomicInt actived_;
    QAtomicInt port_updated_;
    QString port_name_;
    QList<QByteArray> tx_list_;
    QMutex *list_mutex_;
    struct SerialParam param_;
    int send_timeout_;
};


#endif //SERIAL_H_
