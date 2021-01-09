#ifndef XTOOL_H_
#define XTOOL_H_

#include <QObject>

class XToolForm;

class XTool : public QObject {
    Q_OBJECT

public:
    XTool();

private:
    XToolForm *ui_;
};



#endif /* XTOOL_H_ */
