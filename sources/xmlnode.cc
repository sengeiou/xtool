#include "xmlnode.h"

XmlDataNode::XmlDataNode()
{

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


