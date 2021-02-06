#ifndef XTOOL_MODEL_H
#define XTOOL_MODEL_H

#include <QObject>
#include <functional>

#include "observer.h"
#include "xml/xmlparse.h"

class QTimer;
class XmlParse;
class SerialForm;
class SerialThread;
class TransferController;
class StpOpcode;
class XmlDataNode;
class ByteArrayNode;

class XToolModel : public QObject {
    Q_OBJECT

public:
    friend class SerialForm;
    enum {
        XTOOL_EXECUTE_NEXT,
        XTOOL_EXECUTE_TIMEOUT,
        XTOOL_RECVMSG_SHOW,
        XTOOL_SENDMSG_SHOW,
    };

    explicit XToolModel(QObject *parent = nullptr);

    ~XToolModel();
    bool Open();
    bool Send(const QByteArray &buf);
    void ResumeMessageProcess();
    void ExecuteItem(XmlDataNode *item);
    void Stop();
    void CloseSerialPort();
    void SetSerialPort(SerialThread *serial);
    void AddObserver(ObserverBase *obs) {
        observer_list_.AddObserver(obs);
    }
    void RemoveObserver(ObserverBase *obs) {
        observer_list_.RemoveObserver(obs);
    }
    void NotifyObservers(int action, void *ptr) {
        observer_list_.Notify(action, ptr);
    }
    void AttachMessageHandler(const std::function<void(QByteArray *)> &fn) {
        process_fn_ = fn;
    }
    const QList<XmlDataNode *> &filenode() {
        return xml_->root();
    }
    QString *filename() {
        return &filename_;
    }
    SerialThread *serial() const {
        return serial_;
    }
public slots:
    void OnReceiveMessage(ByteArrayNode *);

private slots:
    void OnTimeout();
    void OnRetransmitTimeout();

private:
    bool BuildPacket(XmlDataNode *node, QByteArray *ba, int *timeout);
    void StartTransmitTimer();
    void StopTransmitTimer();
    bool MainMessageProcess(QByteArray *buf);

private:
    enum {
        RETRANS_TIMEOUT = 500, //ms
        RETRANS_COUNT = 2
    };

    ObserverList observer_list_;
    QString filename_;
    SerialThread *serial_;
    XmlParse *xml_;
    StpOpcode *stp_;
    QTimer *timer_;
    QByteArray *waiting_ack_;
    QTimer *retrans_timer_;
    int retrans_count_;
    std::function<void(QByteArray *)> process_fn_;
    XmlDataNode *executing_node_;
};

#endif // XTOOL_MODEL_H
