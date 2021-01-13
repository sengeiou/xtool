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

//    void AppendUint8FromString(const QString &str) {
//        char c = static_cast<char>(str.toUInt());
//        minor_.append(c);
//    }
//    void AppendUint16FromString(const QString &str) {
//        uint v = str.toUInt();
//        char c[2];
//        c[1] = static_cast<char>(v & 0xFF);
//        c[0] = static_cast<char>((v >> 8) & 0xFF);
//        minor_.append(c, 2);
//    }
//    void AppendBytes(const char *s, int size) {
//        minor_.append(s, size);
//    }
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
    QString name_;
    QString major_;
    QString timeout_;
    QList<MinorItem *> minor_;
};

#endif //XML_NODE_H_
