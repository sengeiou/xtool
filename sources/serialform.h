#ifndef SERIALFORM_H_
#define SERIALFORM_H_

#include <QWidget>


class QComboBox;
class QSerialPort;
class QPushButton;
class XToolForm;

class SerialForm : public QWidget {
    Q_OBJECT

public:
    explicit SerialForm(QWidget *parent = nullptr);
    void SetMasterForm(XToolForm *form) {
        master_ = form;
    }

private slots:
    void OnPushButtonClicked();

private:
    void ScanAvaliblePort();
private:
    QComboBox *port_;
    QComboBox *baudrate_;
    QComboBox *databit_;
    QComboBox *stopbit_;
    QComboBox *parity_;
    QPushButton *done_;

    XToolForm *master_;
};



#endif /* SERIALFORM_H_ */
