#include "inthistogramlabel.h"

#include <QSettings>
#include <QFileDialog>
#include <QPainter>
#include <QApplication>
#include <QDebug>

IntHistogramLabel::IntHistogramLabel(int w, int h, int minvalue,int maxvalue) : QLabel()
{
    this->maxvalue = maxvalue ;
    this->minvalue = minvalue ;

image = new QImage(w,h,QImage::Format_ARGB32);
bins_count = (maxvalue-minvalue)+1 ;
bins = new int[bins_count];
for (int i = 0 ; i < bins_count ; i++)
    bins[i] = 0 ;
}

IntHistogramLabel::~IntHistogramLabel()
{
    delete bins ;
    delete image ;
}

void IntHistogramLabel::insertValue(int value)
{
if ((value>=minvalue)&&(value<=maxvalue))
{
    int bin =  value - minvalue ;
    bins[bin] ++ ;
}
}

void IntHistogramLabel::display()
{
    QPainter painter(image) ;
    painter.eraseRect(image->rect());
    int max = 0;
    for(int i = 0 ; i < bins_count ; i++)
        max = qMax(bins[i],max) ;

    for(int i = 0 ; i < bins_count ; i++)
    {
        painter.fillRect(i*(double)image->width()/bins_count,image->height()*(1-bins[i]/(qreal)max),
                         image->width()/(double)bins_count,image->height()*(bins[i]/(qreal)max),Qt::black);
    qDebug() << "histo " << i << " " << bins[i] ;
    }

    for(int i = 0 ; i < bins_count ; i++)
    {
        painter.drawLine((image->width()*i)/bins_count,0,(image->width()*i)/bins_count,image->height());
        painter.drawText((image->width()*i)/bins_count,15,tr("%1").arg(i+minvalue));
    }

    int hgrads = 10 ;
    for(int i = 0 ; i <= hgrads ; i++)
    {
        painter.drawLine(0,(image->height()*(hgrads-i))/hgrads,image->width(),(image->height()*(hgrads-i))/hgrads);
        painter.drawText(0,(image->height()*(hgrads-i))/hgrads+15,QString("%1").arg((int)max*i/hgrads));
    }

    setPixmap(QPixmap::fromImage(*image)) ;
    showNormal() ;
    raise();

}

void IntHistogramLabel::saveToDisk()
{
    QSettings settings ;
    QString last_path = settings.value("last_histogram_saved_to_disk",QApplication::applicationDirPath()).toString() ;
    QString path = QFileDialog::getSaveFileName(this,tr("Choose where you want to write the image"),last_path,
                                                tr("Images (*.png *.xpm *.jpg)"));

    if (path!="")
    {
        image->save(path);
        settings.setValue("last_histogram_saved_to_disk",path);
    }
}
