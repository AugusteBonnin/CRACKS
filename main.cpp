
#include "histopage.h"
#include "mapparamform.h"
#include "mainwindow.h"
#include <QApplication>
#include <QSurfaceFormat>

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(resources);

    QSurfaceFormat format;
        format.setVersion(2, 1);
        format.setProfile(QSurfaceFormat::CoreProfile);
        QSurfaceFormat::setDefaultFormat(format);


       QApplication a(argc, argv);



    MainWindow w;

    return a.exec();
}
