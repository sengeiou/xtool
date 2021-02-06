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
    enum {
        SEND_COMPLETED = 0,
        SEND_FAILED    = 1,
        SEND_PACKET    = 2,
        RECV_PACKET    = 3,
    };
    explicit TestModel(XToolModel *model, QObject *parent = nullptr);
    ~TestModel();

    bool StartCalling(const QString &name, const QString &phone);
    bool SendTextMessage(const QString &name, const QString &phone, int type, const QString &text);
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
    bool SendPacket(int major, int minor, const char *buffer, quint16 len);
    bool ReceiveProcess(QByteArray *buf);
private slots:
    void OnTimeout();

private:
    ObserverList observer_;
    StpOpcode *stp_;
    QByteArray *txbuf_;
    QTimer *timer_;
    XToolModel *xmodel_;
    int retry_;
    int major_;
    int minor_;
};

#endif // TEST_MODEL_H
