#ifndef TRANSFERFORM_H_
#define TRANSFERFORM_H_

#include <QWidget>
#include <QString>

class QPushButton;
class QLineEdit;
class QProgressBar;

class TransferForm : public QWidget {
    Q_OBJECT

public:
    TransferForm(QWidget *parent = nullptr);
    ~TransferForm() = default;

private slots:
    void OnOpenFile();
    void OnSendFile();

private:
    QString filename_;
    QPushButton *btn_open_;
    QPushButton *btn_send_;
    QProgressBar *percent_bar_;
    QLineEdit *fname_ledit_;
};


#endif //TRANSFERFORM_H_
