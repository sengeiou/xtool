#include <QComboBox>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QTextEdit>

#include "testform_view.h"

#include "ui_testform.h"
#include "ui_callform.h"
#include "ui_deviceform.h"
#include "ui_messageform.h"

TestFormView::TestFormView(XToolView *mview, QDialog *parent)
    : QDialog(parent),
      ui_(new Ui::TestFormDialog),
      master_view_(mview),
      child_(nullptr)
{
    ui_->setupUi(this);
}

TestFormView::~TestFormView()
{
    delete ui_;
}

void TestFormView::SetChildWidget(QWidget *view) {
    if (child_) {
        child_->setParent(nullptr);
        ui_->verticalLayout->removeWidget(child_);
        delete child_;
    }
    child_ = view;
    if (view != nullptr)
        ui_->verticalLayout->addWidget(view);
}

//Calling
TestCallView::TestCallView(QWidget *parent)
    : QWidget(parent),
      ui_(new Ui::CallingTest)
{
    ui_->setupUi(this);
}

TestCallView::~TestCallView()
{
    delete ui_;
}

QString TestCallView::name() const
{
    return ui_->combobox_name_->currentText();
}

QString TestCallView::phone() const
{
    return ui_->combobox_phone_->currentText();
}

QPushButton *TestCallView::button() const
{
    return ui_->pushbutton_test_;
}

//Message
TestMessageView::TestMessageView(QWidget *parent)
    : QWidget(parent),
      ui_(new Ui::MessageTest)
{
    ui_->setupUi(this);
}

TestMessageView::~TestMessageView()
{
    delete ui_;
}

QString TestMessageView::name() const
{
    return ui_->combobox_name_->currentText();
}

QString TestMessageView::phone() const
{
    return ui_->combobox_phone_->currentText();
}

int TestMessageView::type() const
{
    return ui_->combobox_type_->currentIndex();
}

QString TestMessageView::message() const
{
    return ui_->text_edit_->toPlainText();
}

QPushButton *TestMessageView::button() const
{
    return ui_->pushbutton_send_;
}

//Device
TestDeviceView::TestDeviceView(QWidget *parent)
    : QWidget(parent),
      ui_(new Ui::DeviceInfoTest)
{
    ui_->setupUi(this);
}

TestDeviceView::~TestDeviceView()
{
    delete ui_;
}

QPushButton *TestDeviceView::button() const
{
    return ui_->pushbutton_;
}

QPushButton *TestDeviceView::clear_button() const
{
    return ui_->pushbutton_clear_;
}

void TestDeviceView::ShowText(const QString &text) const
{
    ui_->text_browser_->append(text);
}

void TestDeviceView::Clear()
{
    ui_->text_browser_->clear();
}
