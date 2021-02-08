#ifndef TEST_CONTROLLER_H
#define TEST_CONTROLLER_H

#include <functional>
#include <QObject>
#include "observer.h"

class TestFormView;
class TestCallView;
class TestModel;

class TestController : public QObject, public ObserverBase {
    Q_OBJECT

public:
    explicit TestController(TestFormView *view, TestModel *model,
                            QObject *parent = nullptr);
    ~TestController();
    void Update(int action, void *ptr) override;
    void Run();
private:
    void Completed();

private slots:
    void OnTestItemActived(int index);
    void OnStartCalling(void);
    void OnSendTextMessage(void);
    void OnReadDeviceInformation(void);

private:
    TestFormView *view_;
    TestModel *model_;
    std::function<void(void)> done_fn_;
};

#endif // TEST_CONTROLLER_H
