#ifndef SERIAL_WORKER_H_
#define SERIAL_WORKER_H_

#include <QObject>
#include <QThread>


class QSerialPort;

class SerialWorker : public QObject {
    Q_OBJECT

public:
    explicit SerialWorker(void);

public slots:
    void StartWork();
    void StopWork();

private:
    QSerialPort *com_;
};

class SerialController : public QObject {
    Q_OBJECT

public:
    SerialController(QObject *parent = nullptr);
    ~SerialController();
    void AddWorker(SerialWorker *worker);
    void RemoveWorker(SerialWorker *worker);
    bool Start();

private:
    QThread thread_worker_;
};

#endif // SERIAL_WORKER_H_
