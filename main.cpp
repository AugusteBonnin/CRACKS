#include "cartoparamform.h"
#include "histopage.h"
#include "mapparamform.h"
#include "mainwindow.h"
#include <QApplication>
#include <QSurfaceFormat>

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(images);

    QSurfaceFormat format;
        format.setDepthBufferSize(0);
        format.setStencilBufferSize(0);
        format.setAlphaBufferSize(8);
        format.setVersion(2, 0);
        format.setProfile(QSurfaceFormat::CoreProfile);
        QSurfaceFormat::setDefaultFormat(format);

    QApplication a(argc, argv);

    MainWindow w;
    w.showMaximized();

    return a.exec();
}
