#ifndef XTOOLFORM_H_
#define XTOOLFORM_H_

#include <QMainWindow>
#include <QString>

class QAction;
class QSerialPort;
class QPushButton;
class QLineEdit;
class QListView;
class QListWidget;
class ByteArray;
class QTimer;
class QCheckBox;

class XmlParse;
class SerialForm;
class SerialThread;
class TransferForm;
class QTextBrowser;
class QListWidgetItem;
class StpOpcode;
class ByteArrayNode;

class XToolForm : public QMainWindow
{
    Q_OBJECT

    friend class SerialForm;
public:
    explicit XToolForm(QWidget *parent = nullptr);
    ~XToolForm();

    bool SendMessage(const QByteArray &buf);
    void ClosePort();

private slots:
    void OnActionConnect();
    void OnActionTransfer();
    void OnOpenFile();
    void OnListActived(QListWidgetItem *item);
    void OnReceiveMessage(ByteArrayNode *);
    void OnTimeout();
    void OnStartExecute();
    void OnStopExecute();
    void OnExecuteItemChanged(QListWidgetItem *, QListWidgetItem *);
    void PortChangedStatus(const QString &s);
    void ProvideContextMenu(const QPoint &point);
    void OnRetransmitTimeout();
private:
    void StartExecute(QListWidgetItem *curr, bool walk_around);
    void StopExecute(QListWidgetItem *curr);
    bool ExecuteNextItem();
    void CreateWidgetList();
    bool BuildPacket(QListWidgetItem *item, QByteArray *ba, int *timeout);
    void ExecuteCurrentItem(QListWidgetItem *item, bool walk_around);
    void ShowCurrentItem(QListWidgetItem *item);
    void MarkingErrorItem(QListWidgetItem *item);
    void StartTransmitTimer();
    void StopTransmitTimer();

private:
    enum {
        RETRANS_TIMEOUT = 500, //ms
        RETRANS_COUNT = 2
    };
    QString filename_;
    QAction *open_;
    QAction *exit_;
    QAction *conn_;
    QAction *about_;
    QAction *transfer_;
    QListWidget *list_widget_;
    QTextBrowser *text_browser_;
    QPushButton *start_btn_;
    QPushButton *stop_btn_;
    QCheckBox *skip_chkbox_;

    TransferForm *transfer_form_;
    SerialThread *serial_;
    XmlParse *xml_;
    StpOpcode *stp_;
    QTimer *timer_;

    QByteArray *waiting_ack_;
    QTimer *retrans_timer_;
    int retrans_count_;
};

#endif //XTOOLFORM_H_
