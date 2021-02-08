#include <QComboBox>
#include <QPushButton>

#include "test_controller.h"
#include "testform_view.h"
#include "test_model.h"
#include "xtool_view.h"

#include "ui_testform.h"
#include "ui_callform.h"
#include "ui_messageform.h"
#include "ui_deviceform.h"

#include "protobuf/devinfo.pb.h"


TestController::TestController(TestFormView *view, TestModel *model, QObject *parent)
    : QObject(parent), view_(view), model_(model)
{
    connect(view_->ui_->combobox_type_, (void (QComboBox::*)(int))&QComboBox::activated,
            this, &TestController::OnTestItemActived);
    model->AddObserver(this);
}

TestController::~TestController()
{
    model_->RemoveObserver(this);
    delete view_;
    delete model_;
}

void TestController::Update(int action, void *ptr)
{
    switch (action) {
    case TestModel::SEND_COMPLETED:
    case TestModel::SEND_FAILED: {
        const QString *str = (const QString *)ptr;
        view_->master_view_->AppendText(*str);
        }
        break;
    case TestModel::SEND_PACKET: {
        const QByteArray *ba = (const QByteArray *)ptr;
        QByteArray str(ba->toHex(' ').toUpper());
        view_->master_view_->AppendText("Send Packet: ");
        view_->master_view_->AppendText(str);
        }
        break;
    case TestModel::RECV_PACKET: {
        const QByteArray *ba = (const QByteArray *)ptr;
        QByteArray str(ba->toHex(' ').toUpper());
        view_->master_view_->AppendText("Receive Packet: ");
        view_->master_view_->AppendText(str);
        }
        break;
    case TestModel::DEVINFO_PACKET: {
        info::Device *dinfo = (info::Device *)ptr;
        TestDeviceView *devinfo = static_cast<TestDeviceView *>(view_->child());
        if (dinfo->has_type()) {
            if (dinfo->type() == info::Device_Type_WATCH)
                devinfo->ShowText("Device type: Smart Watch");
        }
        if (dinfo->has_model()) {
            if (dinfo->model() == info::Device_Model_DM_APOLLO3P)
                devinfo->ShowText("Device model: Ambiq,apollo3p");
        }
        if (dinfo->has_firmware_ver()) {
            quint32 version = dinfo->firmware_ver();
            quint32 major = (version >> 8) & 0xFF;
            quint32 minor = version &  0xFF;
            QString ver = QString("v%1. %2").arg(major).arg( minor);
            devinfo->ShowText("Firmware: " + ver);
        }
        if (dinfo->has_hardware_ver()) {
            quint32 version = dinfo->hardware_ver();
            quint32 major = (version >> 8) & 0xFF;
            quint32 minor = version &  0xFF;
            QString ver = QString("v%1. %2").arg(major).arg( minor);
            devinfo->ShowText("Hardware: " + ver);
        }
        if (dinfo->has_mac()) {
            QByteArray ba = QByteArray::fromStdString(dinfo->mac());
            QByteArray str(ba.toHex(' ').toUpper());
            devinfo->ShowText("MAC Address: " + QString(str));
        }
        }
        break;
    default:
        break;
    }

    done_fn_();
}

void TestController::Run()
{
    view_->exec();
}

void TestController::OnTestItemActived(int index)
{
    if (index == TestFormView::NONE) {
        view_->SetChildWidget(nullptr);
        view_->setWindowTitle("Test Dialog");
    } else if (index == TestFormView::CALLING) {
        TestCallView *call = new TestCallView(view_);
        view_->SetChildWidget(call);
        connect(call->button(), &QPushButton::clicked,
                this, &TestController::OnStartCalling);
        view_->setWindowTitle("Calling Test");
    } else if (index == TestFormView::MESSAGE) {
        TestMessageView *msg = new TestMessageView(view_);
        view_->SetChildWidget(msg);
        connect(msg->button(), &QPushButton::clicked,
                this, &TestController::OnSendTextMessage);
        view_->setWindowTitle("Message Test");
    } else if (index == TestFormView::DEVINFO) {
        TestDeviceView *devinfo = new TestDeviceView(view_);
        view_->SetChildWidget(devinfo);
        connect(devinfo->button(), &QPushButton::clicked,
                this, &TestController::OnReadDeviceInformation);
        connect(devinfo->clear_button(), &QPushButton::clicked,
                devinfo, &TestDeviceView::Clear);
        view_->setWindowTitle("Device information");
    }
}

void TestController::OnStartCalling()
{
    TestCallView *call = static_cast<TestCallView *>(view_->child());
    call->button()->setEnabled(false);
    done_fn_ = std::bind(&QPushButton::setEnabled, call->button(), true);
    model_->StartCalling(call->name(), call->phone());
}

void TestController::OnSendTextMessage()
{
    TestMessageView *msg = static_cast<TestMessageView *>(view_->child());
    msg->button()->setEnabled(false);
    done_fn_ = std::bind(&QPushButton::setEnabled, msg->button(), true);
    model_->SendTextMessage(msg->name(), msg->phone(), msg->type(), msg->message());
}

void TestController::OnReadDeviceInformation()
{
    TestDeviceView *devinfo = static_cast<TestDeviceView *>(view_->child());
    devinfo->button()->setEnabled(false);
    done_fn_ = std::bind(&QPushButton::setEnabled, devinfo->button(), true);
    model_->ReadDeviceInformation();
}
