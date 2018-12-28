#include "mainwindow.h"
#include "mappage.h"
#include "mapresult.h"

#include <QDir>
#include <QFileInfo>
#include <QPainter>
#include <QSettings>
#include "ui_intmapresult.h"

MapResult::MapResult(QString name,MapPage * parent,int index) : Result(name,parent) , index(index)
  , ui(new Ui::IntMapResult) , mainWindow(parent->getMainWindow())

{
    ui->setupUi(this);

}

void MapResult::saveImage(bool screenshot)
{
    QImage image ;
    QSettings settings;
    QFileInfo file(settings.value("File").toString()) ;
    QString path = tr("%1/Cartes-%2-%3")
            .arg(file.absoluteDir().absolutePath())
            .arg(resultName)
            .arg(file.fileName()) ;
    if (screenshot)
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

    if(image.save(path))
        mainWindow->appendToSavedFiles(tr("%1").arg(path));
    else
        mainWindow->appendToSavedFiles(tr("<FONT COLOR=RED>%1 n'a pas été enregistrée.</FONT>").arg(path));
}


void MapResult::saveSVG()
{

    QSettings settings ;
    QFileInfo file(settings.value("File").toString()) ;
    QString path = tr("%1/Cartes-%2-%3.SVG").arg(file.absoluteDir().absolutePath())
            .arg(resultName).arg(file.baseName()) ;
    widget->saveSVG(path);
    mainWindow->appendToSavedFiles(tr("%1").arg(path));

}


