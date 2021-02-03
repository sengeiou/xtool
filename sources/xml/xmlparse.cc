#include <QtXml>
#include <QFile>
#include <QMessageBox>

#include "xmlparse.h"
#include "xmlnode.h"

XmlParse::XmlParse(const QString &fname)
    : file_name_(fname)
{

}

XmlParse::~XmlParse()
{
    for (XmlDataNode *iter : root_) {
        if (iter) {
            root_.removeOne(iter);
            delete iter;
            iter = nullptr;
        }
    }
}

bool XmlParse::ParseFile()
{
    //Project -> Groups -> Content -> Items -> Item
    QFile file(file_name_);
    if (!file.open(QFile::ReadOnly | QIODevice::Text)) {
        QString info = "Couldn't open";
        info.append(file_name_);
        QMessageBox::information(NULL, "Load XML File Problem", info);
        return false;
    }

    QXmlStreamReader reader(&file);
    while (!reader.atEnd()) {
        // Read next element
        QXmlStreamReader::TokenType token = reader.readNext();
        if (token == QXmlStreamReader::Invalid) {
            reader.raiseError("Invalid node");
            break;
        } else if (token == QXmlStreamReader::StartElement) {
            if (reader.name() == "Project")
                continue;
            if (reader.name() == "Groups")
                ParseGroupsNode(&reader, token);
        } else if (token == QXmlStreamReader::EndElement) {
            if (reader.name() == "Project")
                break;
        }
    }
    bool okay = !reader.hasError();
    if (!okay) {

    }

    reader.clear();
    file.close();
    return okay;
}

void XmlParse::ParseGroupsNode(QXmlStreamReader *reader,
                               QXmlStreamReader::TokenType token)
{
    while (!reader->atEnd()) {
        QStringRef str = reader->name();
        if (token == QXmlStreamReader::Invalid) {
            reader->raiseError("Invalid node");
            break;
        } else if (token == QXmlStreamReader::StartElement) {
            if (str == "Groups")
                goto _next;
            if (str == "Content") {
                ParseContentNode(reader, token);
            } else {
                reader->raiseError("Groups element name error");
                break;
            }
        } else if (token == QXmlStreamReader::EndElement) {
            if (str == "Groups")
                break;
        }
    _next:
        token = reader->readNext();
    }
}

void XmlParse::ParseContentNode(QXmlStreamReader *reader,
                                QXmlStreamReader::TokenType token)
{
    XmlDataNode *node = new XmlDataNode();
    while (!reader->atEnd()) {
        if (token == QXmlStreamReader::Invalid) {
            reader->raiseError("Invalid node");
            break;
        } else if (token == QXmlStreamReader::StartElement) {
            if (reader->name() == "Content") {
                QXmlStreamAttributes attr = reader->attributes();
                if (attr.hasAttribute("name"))
                    node->set_name(attr.value("name").toString());
                goto _next;
            } else if (reader->name() == "WaitTimeout") {
                node->set_timeout(reader->readElementText());
            } else if (reader->name() == "Major") {
                node->set_major(reader->readElementText());
            } else if (reader->name() == "Items") {
                ParseItemsNode(reader, node, token);
            } else {
                reader->raiseError("Content element name error");
                break;
            }
        } else if (token == QXmlStreamReader::EndElement) {
            if (reader->name() == "Content")
                break;
        }
    _next:
        token = reader->readNext();
    }
    if (reader->hasError()) {
        delete node;
        return;
    }
    root_.append(node);
}

void XmlParse::ParseItemsNode(QXmlStreamReader *reader,
                              XmlDataNode *node,
                              QXmlStreamReader::TokenType token)
{
    while (!reader->atEnd()) {
        QStringRef str = reader->name();
        if (token == QXmlStreamReader::Invalid) {
            reader->raiseError("Invalid node");
            break;
        } else if (token == QXmlStreamReader::StartElement) {
            if (str == "Items")
                goto _next;
            if (str == "Item") {
                ParseItemNode(reader, node, token);
            } else {
                reader->raiseError("Item element name error");
                break;
            }
        } else if (token == QXmlStreamReader::EndElement) {
            if (str == "Items")
                break;
        }
    _next:
        token = reader->readNext();
    }
}

void XmlParse::ParseItemNode(QXmlStreamReader *reader,
                             XmlDataNode *node,
                             QXmlStreamReader::TokenType token)
{
    XmlDataNode::MinorItem *minor = new XmlDataNode::MinorItem();
    while (!reader->atEnd()) {
        QStringRef str = reader->name();
        if (token == QXmlStreamReader::Invalid) {
            reader->raiseError("Invalid node");
            delete minor;
            return;
        } else if (token == QXmlStreamReader::StartElement) {
            if (str == "Item") {
                goto _next;
            } else if (str == "Minor") {
                minor->minor = reader->readElementText();
            } else if (str == "Data") {
                minor->data = reader->readElementText();
            } else if (str == "Expect") {
                minor->expect = reader->readElementText();
            } else {
                reader->raiseError("Element name error");
                delete minor;
                return;
            }
        } else if (token == QXmlStreamReader::EndElement) {
            if (str == "Item")
                break;
        }
    _next:
        token = reader->readNext();
    }
    node->AppendMinor(minor);
}
