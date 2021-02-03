#include "xmlwidget.h"

XmlWidgetItem::XmlWidgetItem(XmlDataNode *node,
    QListWidget *view)
    : QListWidgetItem(node->name(), view),
      data_(node)
{
}

void XmlWidgetItem::OnItemActived()
{

}

