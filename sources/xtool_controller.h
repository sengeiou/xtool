#ifndef XTOOL_CONTROLLER_H_
#define XTOOL_CONTROLLER_H_

#include <QObject>
#include <QList>

#include "observer.h"

class XToolView;
class XToolModel;
class TransferController;
class SerialForm;
class XmlDataNode;
class XmlWidgetItem;

class QListWidgetItem;
class QPoint;
class QTextBrowser;
class QString;


class XToolController : public QObject, public ObserverBase {
    Q_OBJECT

public:
    explicit XToolController(XToolView *view,
                             XToolModel *model,
                             QObject *parent = nullptr);
    ~XToolController();

    void Update(int action, void *ptr) override;
    void Show();

private:
    void GenerateDataList(const QList<XmlDataNode *> &root);
    void ExecuteItem(XmlWidgetItem *item);
    void ShowItem(QListWidgetItem *item, QTextBrowser *browser, bool clear);
    void AddInformationHeader(QTextBrowser *browser, const QString &info);
    void GenerateStatisticsResult(QTextBrowser *browser);
public slots:
    void OnPortChangedStatus(const QString &s, bool open);
private slots:
    void OnOpenProjectFile();
    void OnOpenSerialPort();
    void OnOPenFileTransfer();
    void OnCloseSerialPort();
    void OnListItemActived(QListWidgetItem *item);
    void OnStartExecute();
    void OnStopExecute();

    void OnListContextMenu(const QPoint &point);

private:
    XToolView *view_;
    XToolModel *model_;
    SerialForm *serial_form_;
    TransferController *transfer_controller_;
};

#endif // XTOOL_CONTROLLER_H_
