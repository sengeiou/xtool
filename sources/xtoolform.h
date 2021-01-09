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

class XmlParse;
class SerialForm;
class SerialThread;
class TransferForm;
class QTextBrowser;
class QListWidgetItem;

class XToolForm : public QMainWindow
{
    Q_OBJECT

    friend class SerialForm;
public:
    explicit XToolForm(QWidget *parent = nullptr);
    ~XToolForm();

private slots:
    void OnActionConnect();
    void OnActionTransfer();
    void OnOpenFile();

    void OnListActived(QListWidgetItem *item);


private:
    void CreateWidgetList();

private:
    QString filename_;
    QAction *open_;
    QAction *exit_;
    QAction *conn_;
    QAction *about_;
    QAction *transfer_;
    QListWidget *list_widget_;
    QTextBrowser *text_browser_;

    XmlParse *xml_;
    TransferForm *transfer_form_;
    SerialThread *serial_;
};

#endif //XTOOLFORM_H_
