#include "xmlwidget.h"

XmlWidgetItem::XmlWidgetItem(XmlDataNode *node,
    QListWidget *view)
    : QListWidgetItem(node->name(), view),
      result_(),
      data_(node),
      breakpoint_enabled_(false),
      result_okay_(true)
{
}

void XmlWidgetItem::OnItemActived()
{

}

