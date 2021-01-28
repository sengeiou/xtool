#ifndef XTOOL_VIEW_H_
#define XTOOL_VIEW_H_

#include <QMainWindow>
#include <QStringList>

#include "observer.h"
#include "ui_xtoolform.h"




namespace Ui {
class MainWindow;
}

class XToolController;
class QLabel;
class QPushButton;
class QListWidgetItem;
class QPoint;

class XToolView : public QMainWindow {
    Q_OBJECT

public:
    friend class XToolController;
    explicit XToolView(QWidget *parent = nullptr);
    ~XToolView();
    bool OpenFileSystem(QString *filename,
                        const QString &filter,
                        const QString &title="Select File");
    void CreateListWidget(QListWidgetItem *item);
    void StartListProcess(QListWidgetItem *item);
    void StopListProcess(QListWidgetItem *item);
    void SetSerialPortStatus(bool ready);
    void SetStatusBarText(const QString &text);
    void ClearLogText();
    void ClearListWidget();
    QListWidgetItem *current() const;
    QListWidgetItem *next() const;
    QListWidgetItem *item_at(const QPoint &);

private slots:
    void OnExecuteItemChanged(QListWidgetItem *prev, QListWidgetItem *curr);

private:
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
};

#endif // XTOOL_VIEW_H_
