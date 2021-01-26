#ifndef TRANSFER_CONTROLLER_H_
#define TRANSFER_CONTROLLER_H_

#include <QtCore/qobject.h>
#include "observer.h"

class TransferView;
class FileTransfer;

class TransferController: public QObject, public ObserverBase {
    Q_OBJECT
public:
    TransferController(TransferView *view, FileTransfer *transfer);
    virtual ~TransferController();
    void MessageProcess(QByteArray *buf);
    void Show();
    void Update(int action, void *ptr) override;

private slots:
    void OnOpenFile();
    void OnTransferFile();

private:
    TransferView *view_;
    FileTransfer *file_transfer_;
    bool actived_;
};


#endif //TRANSFER_CONTROLLER_H_
