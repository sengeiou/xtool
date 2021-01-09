#ifndef XMLPARSE_H_
#define XMLPARSE_H_

#include <QList>
#include <QByteArrayList>
#include <QtXml>

class XmlDataNode;

class XmlParse {
public:
    XmlParse(const QString &fname);
    ~XmlParse();
    bool ParseFile();
    const QList<XmlDataNode *> &root() {
        return root_;
    }
    const QString &title() {
        return title_;
    }
    void AddXmlNode(XmlDataNode *node) {
        root_.append(node);
    }
    
private:
    void ParseGroupsNode(QXmlStreamReader *reader,
                         QXmlStreamReader::TokenType token);
    void ParseContentNode(QXmlStreamReader *reader,
                          QXmlStreamReader::TokenType token);
    void ParseItemsNode(QXmlStreamReader *reader,
                        XmlDataNode *node,
                        QXmlStreamReader::TokenType token);
    void ParseItemNode(QXmlStreamReader *reader,
                       XmlDataNode *node,
                       QXmlStreamReader::TokenType token);

private:
    QString file_name_;
    QString title_;
    QList<XmlDataNode *> root_;
};

#endif //XMLPARSE_H_
