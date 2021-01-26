#ifndef TRANSFER_VIEW_H_
#define TRANSFER_VIEW_H_

#include <QWidget>
#include "ui_transferform.h"


class QTextBrowser;
class QString;

class TransferView : public QWidget {
    Q_OBJECT
public:
    friend class TransferController;
    TransferView(QWidget *parent, QTextBrowser *text_browser);
    ~TransferView();
    bool OpenFileSystem(QString *filename,
                        const QString &filter,
                        const QString &title="Select File");
    void ShowTransferFile(const QString &name);
    void ShowTransferProcess(quint32 percent);
    void SetTransferButtonStatus(bool enable);
    void ResetProgressBar();
    void SetTransferButtonText(const QString &);
    void SetProgressBarStatus(bool visible);
    void ShowText(const QString &text);

private:
    Ui::TransferForm ui_;
    QTextBrowser *text_browser_;
};

#endif //TRANSFER_VIEW_H_
