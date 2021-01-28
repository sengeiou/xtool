#ifndef XMLWIDGET_H_
#define XMLWIDGET_H_

#include <QListWidget>
#include "xmlnode.h"

class QString;
class MinorItem;

class XmlWidgetItem : public QListWidgetItem {
public:
    explicit XmlWidgetItem(XmlDataNode *node, QListWidget *view = nullptr);
    ~XmlWidgetItem() {
        data_ = nullptr;
    }
    const QString &name() const {
        return data_->name();
    }
    const QString &major() const {
        return data_->major();
    }
    const QString &timeout() const {
        return data_->timeout();
    }
    QList<XmlDataNode::MinorItem *> &minor() {
        return data_->minor();
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
        return result_;
    }
    XmlDataNode *data() {
        return data_;
    }

private slots:
    void OnItemActived();

private:
    QString result_;
    XmlDataNode *data_;
    bool breakpoint_enabled_;
    bool result_okay_;
};



#endif //XMLWIDGET_H_
