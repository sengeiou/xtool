#include "xmlnode.h"

XmlDataNode::XmlDataNode()
    : exec_result_(),
      breakpoint_enabled_(false),
      result_okay_(true)
{
    exec_result_.resize(64);
}

XmlDataNode::~XmlDataNode()
{
    for (MinorItem *iter : minor_) {
        if (iter) {
            minor_.removeOne(iter);
            delete iter;
            iter = nullptr;
        }
    }
}



