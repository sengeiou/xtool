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

    void ResetStatus() {
        result_okay_ = true;
        exec_result_.clear();
    }
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
    void set_breakpoint(bool enable) {
        breakpoint_enabled_ = enable;
    }
    bool breakpoint() const {
        return breakpoint_enabled_;
    }
    void set_result(bool okay) {
        result_okay_ = okay;
    }
    bool result() const {
        return result_okay_;
    }
    const QString &result_info() const {
        return exec_result_;
    }
    void set_result_info(const QString &info) {
        exec_result_.clear();
        exec_result_ = info;
    }

private:
    QList<MinorItem *> minor_;
    QString name_;
    QString major_;
    QString timeout_;

    QString exec_result_;
    bool breakpoint_enabled_;
    bool result_okay_;
};

#endif //XML_NODE_H_
