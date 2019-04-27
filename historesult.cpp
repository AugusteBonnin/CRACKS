#include "historesult.h"

#include <QDir>
#include <QFileInfo>
#include <QPainter>
#include <QSettings>
#include <QTextStream>
#include "histopage.h"
#include "mainwindow.h"

HistoResult::HistoResult(QString name, HistoPage *parent,int index) : Result(name,parent) , index(index)
{
mainWindow = parent->getMainWindow() ;
}


void HistoResult::makeImage()
{
    QSettings settings;
    settings.beginReadArray("Histo");
    settings.setArrayIndex(index);
    {
        image = QPixmap(size());
        QPainter painter;
        painter.begin(&image);
        render(&painter);
    }

}
void HistoResult::saveImage()
{
    image = QPixmap(size());
    QPainter painter;
    painter.begin(&image);
    render(&painter);

    QSettings settings ;
    QFileInfo fileInfo(settings.value("File").toString()) ;
    QDir dir(fileInfo.absoluteDir());

    QString path(dir.filePath((tr("Histogramme-")+resultName+"-"+fileInfo.fileName())))  ;
    if(image.save(path))
       mainWindow->appendToSavedFiles(tr("%1").arg(path));
   else
       mainWindow->appendToSavedFiles(tr("<FONT COLOR=RED>%1 n'a pas été enregistré.</FONT>").arg(path));
}

void HistoResult::saveCSV()
{
    QSettings settings ;
    QFileInfo fileInfo(settings.value("File").toString()) ;
    QDir dir(fileInfo.absoluteDir());

    QString path(dir.filePath((tr("Histogramme-")+resultName+"-"+fileInfo.fileName()+".CSV")))  ;

    QFile file(path);
    if (file.open(QIODevice::WriteOnly|QIODevice::Text))
    {
        QTextStream ts(&file) ;
        ts << csv ;

        file.close() ;
        mainWindow->appendToSavedFiles(tr("%1").arg(path));
    }

}

void HistoResult::paintEvent(QPaintEvent *e)
{
    QPainter painter(this) ;
    painter.setBrush(QBrush(Qt::white));
    painter.drawRect(QRect(0,0,width()-1,height()-1));

    QWidget::paintEvent(e) ;
}
