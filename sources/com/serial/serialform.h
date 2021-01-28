#ifndef SERIALFORM_H_
#define SERIALFORM_H_

#include <QWidget>


class QComboBox;
class QSerialPort;
class QPushButton;
class XToolModel;
class XToolController;

namespace Ui {
class SerialForm;
}

class SerialForm : public QWidget {
    Q_OBJECT

public:
    explicit SerialForm(QWidget *parent = nullptr);
    ~SerialForm();
    void SetMasterForm(XToolModel *model) {
        master_ = model;
    }
    void SetController(XToolController *controller) {
        controller_ = controller;
    }

private slots:
    void OnPushButtonClicked();

private:
    void ScanAvaliblePort();
private:
    Ui::SerialForm *ui_;
    QComboBox *port_;
    QComboBox *baudrate_;
    QComboBox *databit_;
    QComboBox *stopbit_;
    QComboBox *parity_;
    QPushButton *done_;

    XToolModel *master_;
    XToolController *controller_;
};



#endif /* SERIALFORM_H_ */
