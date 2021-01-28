#ifndef SERIAL_H_
#define SERIAL_H_

#include <QThread>
#include <QAtomicInt>
#include <QByteArray>
#include <QList>

#include <QSerialPort>

#include "observer.h"
#include "dlistbuf.h"

class QMutex;
class QTimer;

class SerialThread : public QThread {
    Q_OBJECT

public:
    enum {
        SERIAL_COM_ADD,
    };
    struct SerialParam {
        QSerialPort::BaudRate baudrate;
        QSerialPort::DataBits data_bits;
        QSerialPort::StopBits stop_bits;
        QSerialPort::Parity parity;
    };

    explicit SerialThread(QObject *parent = nullptr);
    ~SerialThread();
    bool Start(const QString &portname);
    void Stop();
    bool SendTo(const QByteArray &);
    void StartPortScan();
    void StopPortScan();
    void SetBaudrate(int index);
    void SetDatabit(int index);
    void SetStopbit(int index);
    void SetParity(int index);

    void AddObserver(ObserverBase *obs) {
        observer_list_.AddObserver(obs);
    }
    void RemoveObserver(ObserverBase *obs) {
        observer_list_.RemoveObserver(obs);
    }
    void NotifyObservers(int action, void *ptr) {
        observer_list_.Notify(action, ptr);
    }
    const QString &port_name() const {
        return port_name_;
    }

public slots:
    void ScanAvaliblePort();
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
    ObserverList observer_list_;
    QAtomicInt actived_;
    QAtomicInt port_updated_;
    QString port_name_;
    ByteArrayList *tx_queue_;
    ByteArrayNodePool *node_pool_;
    QByteArray rxbuf_;
    QMutex *list_mutex_;
    struct SerialParam param_;
    QTimer *port_refresh_timer_;
    int send_timeout_;
};


#endif //SERIAL_H_
