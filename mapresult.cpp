#include "mainwindow.h"
#include "mappage.h"
#include "mapresult.h"

#include <QDir>
#include <QFileInfo>
#include <QPainter>
#include <QSettings>
#include "ui_intmapresult.h"
#include <QtSvg>
#include <QSvgGenerator>

MapResult::MapResult(QString name,MapPage * parent,int index) : Result(name,parent) , index(index)
  , ui(new Ui::IntMapResult) , mainWindow(parent->getMainWindow())

{
    ui->setupUi(this);

}

void MapResult::saveImage()
{
    QImage image ;
    QSettings settings;
    QFileInfo file(settings.value("File").toString()) ;
    QString path = tr("%1/Cartes-%2-%3")
            .arg(file.absoluteDir().absolutePath())
            .arg(resultName)
            .arg(file.fileName()) ;
    settings.beginReadArray("Maps");
    settings.setArrayIndex(index);
    if (settings.value("Screenshot",true).toBool())
    {
        image = QImage(size(),QImage::Format_ARGB32);
        QPainter painter(&image);
        render(&painter);
    }
    else
    {
        QSize newsize = mainWindow->openedQImage.size() ;
        newsize.setHeight(newsize.height()+64);
        newsize.setWidth(newsize.width()+128);
        image = QImage(newsize,QImage::Format_ARGB32);
        QPainter painter(&image);
        QSize oldSize = size() ;
        resize(newsize) ;
        widget->resetToSize(newsize);
        render(&painter);
        resize(oldSize);
        widget->resetToSavedState();
    }
    settings.endArray();
    image.save(path);
}

void MapResult::saveSVG()
{

        QSettings settings ;
        QFileInfo file(settings.value("File").toString()) ;
        widget->saveSVG(tr("%1/Cartes-%2-%3.SVG").arg(file.absoluteDir().absolutePath())
                        .arg(resultName).arg(file.baseName()));

}


