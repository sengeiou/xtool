#ifndef XML_NODE_H_
#define XML_NODE_H_

#include <QByteArray>
#include <QString>
#include <QList>

class XmlDataNode {
public:
    struct MinorItem {
        QString minor;
        QString data;
        QString expect;
    };

    XmlDataNode();
    ~XmlDataNode();

    void AppendMinor(MinorItem *minor) {
        minor_.append(minor);
    }
    void set_name(const QString &s) {
        name_ = s;
    }
    void set_major(const QString &s) {
        major_ = s;
    }
    void set_timeout(const QString &s) {
        timeout_ = s;
    }
    const QString &name() const {
        return name_;
    }
    const QString &major() const {
        return major_;
    }
    const QString &timeout() const {
        return timeout_;
    }
    QList<XmlDataNode::MinorItem *> &minor() {
        return minor_;
    }

private:
    QList<MinorItem *> minor_;
    QString name_;
    QString major_;
    QString timeout_;
};

#endif //XML_NODE_H_
