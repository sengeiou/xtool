#ifndef FILE_TRANSFER_H_
#define FILE_TRANSFER_H_

#include <QString>
#include <QObject>


#include "observer.h"

class QFile;
class QByteArray;
class QTimer;
class OTAHeader;
class StpOpcode;
class XToolModel;
class FileTransfer;

#pragma pack(1)
struct FileTransferContext {
    quint16 maxno;
    quint16 no;
    uint32_t crc;
};

#pragma pack(1)
struct FileRequestStatus {
    quint16 max_file_size; //KB
    quint8 bat_power; //0 - 100
};

class AbstractTransfer {
public:
    virtual ~AbstractTransfer() {}
    virtual bool Send(FileTransfer *form) = 0;
    virtual bool Receive(FileTransfer *form, QByteArray *buf) = 0;
};

class FileRequestState : public AbstractTransfer {
public:
    FileRequestState() = default;
    ~FileRequestState() = default;
    bool Send(FileTransfer *form) override;
    bool Receive(FileTransfer *form, QByteArray *buf) override;
};

class FileBreakpointState : public AbstractTransfer {
public:
    FileBreakpointState() = default;
    ~FileBreakpointState() = default;
    bool Send(FileTransfer *form) override;
    bool Receive(FileTransfer *form, QByteArray *buf) override;
};

class FileSendState : public AbstractTransfer {
public:
    FileSendState() = default;
    ~FileSendState() = default;
    bool Send(FileTransfer *form) override;
    bool Receive(FileTransfer *form, QByteArray *buf) override;
};

class FileStopState : public AbstractTransfer {
public:
    FileStopState() = default;
    ~FileStopState() = default;
    bool Send(FileTransfer *form) override;
    bool Receive(FileTransfer *form, QByteArray *buf) override;
};

class FileNullState : public AbstractTransfer {
public:
    FileNullState() = default;
    ~FileNullState() = default;
    bool Send(FileTransfer *) override {
        return true;
    }
    bool Receive(FileTransfer *, QByteArray *) override {
        return true;
    }
};



class FileTransfer : public QObject {
    Q_OBJECT

public:
    friend class FileRequestState;
    friend class FileBreakpointState;
    friend class FileSendState;
    friend class FileStopState;

    enum {
        TRANSFER_SHOW_PROGRESS,
        TRANSFER_SHOW_TX_PACKET,
        TRANSFER_SHOW_RX_PACKET,
        TRANSFER_SHOW_RETX_PACKET,
        TRANSFER_FINISHED,
        TRANSFER_FAILED,
        TRANSFER_PORT_CLOSED
    };

    explicit FileTransfer(XToolModel *master);
    ~FileTransfer();
    void FileMessageProcess(QByteArray *buf);
    bool StartFileTransfer();
    void StopFileTransfer();
    void CloseProcess();

    void AddObserver(ObserverBase *obs) {
        observer_.AddObserver(obs);
    }
    void RemoveObserver(ObserverBase *obs) {
        observer_.RemoveObserver(obs);
    }
    quint32 size() const {
        return file_size_;
    }
    quint32 crc() const {
        return file_crc_;
    }
    quint32 percent() const {
        return file_percent_;
    }
    QString *filename() {
        return &filename_;
    }

private slots:
    void OnTimeout();

private:
    void CloseFile();
    void ResetOTAHeader(const QFile &);
    quint32 FileChecksum(const quint8 *data, quint32 len, quint32 crc=0);

private:
    bool GenerateReceive(QByteArray *buf, quint8 match_code);
    void SetTranferState(AbstractTransfer *next_state);
    bool SendProcess(void);
    bool ReceiveProcess(QByteArray *buf);

private:
    QString filename_;
    XToolModel *master_;
    QFile *file_;
    QByteArray *txbuf_;
    QTimer *timer_;
    OTAHeader *ota_;
    int tx_retry_;

    StpOpcode *stp_;
    quint32 file_crc_;
    quint32 file_size_;
    qint64 file_sent_size_;
    quint32 file_percent_;
    ObserverList observer_;

private:
    FileRequestState requst_state_;
    FileBreakpointState bkpt_state_;
    FileSendState send_state_;
    FileStopState stop_state_;
    FileNullState null_state_;
    AbstractTransfer *transfer_;
};

#endif // FILE_TRANSFER_H
