#ifndef TRANSFERFORM_H_
#define TRANSFERFORM_H_

#include <QWidget>
#include <QString>

class QPushButton;
class QLineEdit;
class QProgressBar;
class XToolForm;
class QByteArray;
class QFile;
class QTimer;
class QTextBrowser;

class ByteArrayNode;
class StpOpcode;

struct OTAHeader;
class TransferForm;

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
    virtual bool Send(TransferForm *form) = 0;
    virtual bool Receive(TransferForm *form, QByteArray *buf) = 0;
};

class FileRequestState : public AbstractTransfer {
public:
    FileRequestState() = default;
    ~FileRequestState() = default;
    bool Send(TransferForm *form) override;
    bool Receive(TransferForm *form, QByteArray *buf) override;
};

class FileBreakpointState : public AbstractTransfer {
public:
    FileBreakpointState() = default;
    ~FileBreakpointState() = default;
    bool Send(TransferForm *form) override;
    bool Receive(TransferForm *form, QByteArray *buf) override;
};

class FileSendState : public AbstractTransfer {
public:
    FileSendState() = default;
    ~FileSendState() = default;
    bool Send(TransferForm *form) override;
    bool Receive(TransferForm *form, QByteArray *buf) override;
};

class FileStopState : public AbstractTransfer {
public:
    FileStopState() = default;
    ~FileStopState() = default;
    bool Send(TransferForm *form) override;
    bool Receive(TransferForm *form, QByteArray *buf) override;
};


class TransferForm : public QWidget {
    Q_OBJECT

public:
    friend class FileRequestState;
    friend class FileBreakpointState;
    friend class FileSendState;
    friend class FileStopState;
    explicit TransferForm(XToolForm *master, QWidget *parent = nullptr);
    ~TransferForm();
    void FileMessageProcess(QByteArray *buf);

private slots:
    void OnOpenFile();
    void OnSendFile();
    void OnTimeout();

private:
    void CloseFile();
    void ResetOTAHeader(const QFile &);
    void ShowPacket(const QString &title, const QByteArray &text);
    quint32 FileChecksum(const quint8 *data, quint32 len, quint32 crc=0);

private:
    bool GenerateReceive(QByteArray *buf, quint8 match_code);
    void SetTranferState(AbstractTransfer *next_state);
    bool SendProcess(void);
    bool ReceiveProcess(QByteArray *buf);


private:
    QString filename_;
    QPushButton *btn_open_;
    QPushButton *btn_send_;
    QProgressBar *percent_bar_;
    QLineEdit *fname_ledit_;

    XToolForm *master_;
    QFile *file_;
    QByteArray *txbuf_;
    QTimer *timer_;
    OTAHeader *ota_;
    int tx_retry_;

    StpOpcode *stp_;
    quint32 file_crc_;
    quint32 file_size_;
    qint64 file_sent_size_;
    
private:
    QTextBrowser *text_browser_;
    FileRequestState requst_state_;
    FileBreakpointState bkpt_state_;
    FileSendState send_state_;
    FileStopState stop_state_;
    AbstractTransfer *transfer_;
};


#endif //TRANSFERFORM_H_
