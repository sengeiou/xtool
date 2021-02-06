#include <QComboBox>
#include <QPushButton>

#include "test_controller.h"
#include "testform_view.h"
#include "test_model.h"
#include "xtool_view.h"

#include "ui_testform.h"
#include "ui_callform.h"
#include "ui_messageform.h"


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
    if (action == TestModel::SEND_COMPLETED ||
        action == TestModel::SEND_FAILED) {
        const QString *str = (const QString *)ptr;
        view_->master_view_->AppendText(*str);
    } else if (action == TestModel::SEND_PACKET) {
        const QByteArray *ba = (const QByteArray *)ptr;
        QByteArray str(ba->toHex(' ').toUpper());
        view_->master_view_->AppendText("Send Packet: ");
        view_->master_view_->AppendText(str);
    } else if (action == TestModel::RECV_PACKET) {
        const QByteArray *ba = (const QByteArray *)ptr;
        QByteArray str(ba->toHex(' ').toUpper());
        view_->master_view_->AppendText("Receive Packet: ");
        view_->master_view_->AppendText(str);
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
