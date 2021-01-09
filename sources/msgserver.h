#ifndef MESSAGE_SERVER_H_
#define MESSAGE_SERVER_H_

#include <QByteArray>

class MessageServer {
public:
    MessageServer() = default;
    virtual ~MessageServer() {}
    virtual void ReceiveMessage(QByteArray &) = 0;

private:
    Q_DISABLE_COPY(MessageServer)
};


#endif //MESSAGE_SERVER_H_
