#include <QApplication>

#include "xtool_controller.h"
#include "xtool_view.h"
#include "xtool_model.h"

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(xtool);

    QApplication app(argc, argv);
    XToolView view;
    XToolModel model;
    XToolController controller(&view, &model);
    controller.Show();
    return app.exec();
}
