#ifndef TESTFORM_VIEW_H_
#define TESTFORM_VIEW_H_

#include <QDialog>
#include <QWidget>

class QGridLayout;
class QPushButton;
class XToolView;

namespace Ui {
    class TestFormDialog;
    class CallingTest;
    class MessageTest;
    class DeviceInfoTest;
} // namespace Ui


//Calling class
class TestCallView : public QWidget {
    Q_OBJECT

public:
    explicit TestCallView(QWidget *parent = nullptr);
    ~TestCallView();
    QString name() const;
    QString phone() const;
    QPushButton *button() const;
private:
    Ui::CallingTest *ui_;
};


//Message class
class TestMessageView : public QWidget {
    Q_OBJECT

public:
    explicit TestMessageView(QWidget *parent = nullptr);
    ~TestMessageView();

    QString name() const;
    QString phone() const;
    int type() const;
    QString message() const;
    QPushButton *button() const;
private:
    Ui::MessageTest *ui_;
};

//Device class
class TestDeviceView : public QWidget {
    Q_OBJECT

public:
    explicit TestDeviceView(QWidget *parent = nullptr);
    ~TestDeviceView();

    QPushButton *button() const;
    QPushButton *clear_button() const;
    void ShowText(const QString &text) const;
    void Clear();
private:
    Ui::DeviceInfoTest *ui_;
};


class TestFormView : public QDialog {
    Q_OBJECT

public:
    enum {
        NONE    = 0,
        CALLING = 1,
        MESSAGE = 2,
        SETTING = 3,
        DEVINFO = 4,
    };
    friend class TestController;
    explicit TestFormView(XToolView *mview, QDialog *parent = nullptr);
    ~TestFormView();

    void SetChildWidget(QWidget *view);
    QWidget *child() const {
        return child_;
    }

private slots:


private:
    Ui::TestFormDialog *ui_;
    XToolView *master_view_;
    QWidget *child_;
};

#endif // TESTFORM_VIEW_H_
