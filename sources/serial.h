#ifndef SERIAL_H_
#define SERIAL_H_

#include <QThread>
#include <QAtomicInt>
#include <QByteArray>
#include <QList>

#include <QSerialPort>

#include "dlistbuf.h"

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
    void Connected(const QString &, bool);
    void Disconnected(const QString &, bool);
    void Error(const QString &);
    void RecvMessage(ByteArrayNode *);

protected:
    virtual void run() override;

private:
    void RequestUpdatePort() {
        port_updated_.store(1);
    }
    ByteArrayNode *AllocateNode(bool wait = true) {
        return node_pool_->Allocate(wait);
    }
    void ReleaseNode(ByteArrayNode *node) {
        node_pool_->Release(node);
    }

private:
    QAtomicInt actived_;
    QAtomicInt port_updated_;
    QString port_name_;
    //QList<QByteArray> tx_list_;

    ByteArrayList *tx_queue_;
    ByteArrayNodePool *node_pool_;

    QByteArray rxbuf_;
    QMutex *list_mutex_;
    struct SerialParam param_;
    int send_timeout_;
};


#endif //SERIAL_H_
