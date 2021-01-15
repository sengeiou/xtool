#ifndef SERIALASYNC_H_
#define SERIALASYNC_H_

#include <QThread>
#include <QByteArray>
#include <QSerialPort>

class QSerialPort;
class QByteArray;

class SerialWorker : public QOject {
    Q_OBJECT

public:
    SerialWorker(QObject *parent = nullptr);
    ~SerialWorker();

private slots:
    void OnSendMessage(const QByteArray &);

private:
    QThread *worker_;
    QSerialPort *port_;
};


#endif //SERIALASYNC_H_
