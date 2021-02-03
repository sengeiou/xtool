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
        data_->set_breakpoint(enable);
    }
    bool breakpoint() const {
        return data_->breakpoint();
    }
    void set_result(bool okay) {
        data_->set_result(okay);
    }
    bool result() const {
        return data_->result();
    }
    const QString &result_info() const {
        return data_->result_info();
    }
    XmlDataNode *data() {
        return data_;
    }

private slots:
    void OnItemActived();

private:
    XmlDataNode *data_;
};



#endif //XMLWIDGET_H_
