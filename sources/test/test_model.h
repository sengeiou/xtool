#ifndef TEST_MODEL_H
#define TEST_MODEL_H

#include <QObject>
#include "observer.h"

class QString;
class XToolModel;
class StpOpcode;
class QByteArray;
class QTimer;
class SerialThread;

class TestModel : public QObject {
    Q_OBJECT
public:
    typedef bool (TestModel::*receive_fn_t)(const quint8 *, quint16);
    enum {
        SEND_COMPLETED = 0,
        SEND_FAILED    = 1,
        SEND_PACKET    = 2,
        RECV_PACKET    = 3,
        DEVINFO_PACKET = 4,
    };
    explicit TestModel(XToolModel *model, QObject *parent = nullptr);
    ~TestModel();

    bool StartCalling(const QString &name, const QString &phone);
    bool SendTextMessage(const QString &name, const QString &phone, int type, const QString &text);
    bool ReadDeviceInformation(void);
    void AddObserver(ObserverBase *obs) {
        observer_.AddObserver(obs);
    }
    void RemoveObserver(ObserverBase *obs) {
        observer_.RemoveObserver(obs);
    }
    void Notify(int action, void *ptr) {
        observer_.Notify(action, ptr);
    }

private:
    void SetReceiveProcess(receive_fn_t fn) {
        recv_fn_ = fn;
    }

    bool SendPacket(int major, int minor, const char *buffer, quint16 len);
    bool ReceiveProcess(QByteArray *buf);
    bool UnpackDeviceInformation(const quint8 *packet, quint16 len);
private slots:
    void OnTimeout();

private:
    ObserverList observer_;
    StpOpcode *stp_;
    QByteArray *txbuf_;
    QTimer *timer_;
    XToolModel *xmodel_;
    receive_fn_t recv_fn_;
    int retry_;
    int major_;
    int minor_;
};

#endif // TEST_MODEL_H
