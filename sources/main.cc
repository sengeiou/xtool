#include <QApplication>

#include "xtoolform.h"

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(xtool);

    QApplication app(argc, argv);
    XToolForm xtool;
    xtool.show();
    return app.exec();
}
