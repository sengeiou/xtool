#include "xmlwidget.h"

XmlWidgetItem::XmlWidgetItem(XmlDataNode *node,
    QListWidget *view)
    : QListWidgetItem(node->name(), view),
      data_(node),
      breakpoint_enabled_(false)
{

}

void XmlWidgetItem::OnItemActived()
{

}

