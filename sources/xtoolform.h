#ifndef XTOOLFORM_H_
#define XTOOLFORM_H_

#include <QMainWindow>
#include <QString>
#include <functional>

class QAction;
class QSerialPort;
class QPushButton;
class QLineEdit;
class QListView;
class QListWidget;
class ByteArray;
class QTimer;
class QCheckBox;
class QLabel;

class XmlParse;
class SerialForm;
class SerialThread;
class TransferController;
class QTextBrowser;
class QListWidgetItem;
class StpOpcode;
class ByteArrayNode;

namespace Ui {
class MainWindow;
}

class XToolForm : public QMainWindow {
    Q_OBJECT

    friend class SerialForm;
    friend class TransferForm;
public:
    explicit XToolForm(QWidget *parent = nullptr);
    ~XToolForm();

    bool Send(const QByteArray &buf);
    void ResumeMessageProcess();

private slots:
    void OnClosePort();
    void OnActionConnect();
    void OnActionTransfer();
    void OnOpenFile();
    void OnListActived(QListWidgetItem *item);
    void OnReceiveMessage(ByteArrayNode *);
    void OnTimeout();
    void OnStartExecute();
    void OnStopExecute();
    void OnExecuteItemChanged(QListWidgetItem *, QListWidgetItem *);
    void OnPortChangedStatus(const QString &s, bool open);
    void ProvideContextMenu(const QPoint &point);
    void OnRetransmitTimeout();
    void OnClearText();

private:
    void StartExecute(QListWidgetItem *curr, bool walk_around);
    void StopExecute(QListWidgetItem *curr);
    bool ExecuteNextItem();
    void CreateWidgetList();
    bool BuildPacket(QListWidgetItem *item, QByteArray *ba, int *timeout);
    void ExecuteCurrentItem(QListWidgetItem *item, bool walk_around);
    void ShowCurrentItem(QListWidgetItem *item, QTextBrowser *text, bool clear);
    void MarkingErrorItem(QListWidgetItem *item);
    void StartTransmitTimer();
    void StopTransmitTimer();
    void GenerateResult();
    void AddInformationHeader(QTextBrowser *text, const QString &info);
    void MainMessageProcess(QByteArray *buf);

private:
    enum {
        RETRANS_TIMEOUT = 500, //ms
        RETRANS_COUNT = 2
    };
    QString filename_;
    Ui::MainWindow *ui_;
    QAction *open_;
    QAction *exit_;
    QAction *conn_;
    QAction *about_;
    QAction *transfer_;
    QListWidget *list_widget_;
    QTextBrowser *text_browser_;
    QTextBrowser *item_text_browser_;
    QPushButton *start_btn_;
    QPushButton *stop_btn_;
    QCheckBox *skip_chkbox_;
    QLabel *status_label_;

    //Toolbar action
    QAction *toolbar_conn_;
    QAction *toolbar_disconn_;
    QAction *toolbar_setting_;
    QAction *toolbar_clear_;

    TransferController *transfer_controller_;
    SerialThread *serial_;
    SerialForm *serial_form_;
    XmlParse *xml_;
    StpOpcode *stp_;
    QTimer *timer_;

    QByteArray *waiting_ack_;
    QTimer *retrans_timer_;
    int retrans_count_;

    std::function<void(QByteArray *)> process_fn_;
};

#endif //XTOOLFORM_H_
