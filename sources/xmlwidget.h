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
private slots:
    void OnItemActived();

private:
    XmlDataNode *data_;
    bool need_stop_;
};


#endif //XMLWIDGET_H_
