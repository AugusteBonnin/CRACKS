#include "histogramlabel.h"

#include <QSettings>
#include <QFileDialog>
#include <QPainter>
#include <QApplication>

HistogramLabel::HistogramLabel(int w, int h, int maxvalue, int step, int wgrads, int hgrads) : QLabel()
{
    this->maxvalue = maxvalue ;
    this->step = step ;
    this->wgrads = wgrads ;
    this->hgrads = hgrads ;
image = new QImage(w,h,QImage::Format_ARGB32);
bins_count = (maxvalue/step)+1 ;
bins = new int[bins_count];
for (int i = 0 ; i < bins_count ; i++)
    bins[i] = 0 ;
}

HistogramLabel::~HistogramLabel()
{
    delete bins ;
    delete image ;
}

void HistogramLabel::insertValue(double value)
{
if ((value>=0)&&(value<=maxvalue))
{
    int bin = (int)(bins_count * value / maxvalue) ;
    if (bin == bins_count) bin-- ;
    bins[bin] ++ ;
}
}

void HistogramLabel::display()
{
    QPainter painter(image) ;
    painter.eraseRect(image->rect());
    double max = 0;
    for(int i = 0 ; i < bins_count ; i++)
        max = qMax((double)bins[i],max) ;

    for(int i = 0 ; i < bins_count ; i++)
        painter.fillRect(i*(double)image->width()/(bins_count-1),image->height()*(1-bins[i]/max),image->width()/(double)bins_count,image->height()*(bins[i]/max),Qt::black);

    //painter.setCompositionMode(QPainter::CompositionMode_Xor);
    //painter.setBrush(QBrush(Qt::black));
    for(int i = 0 ; i <= wgrads ; i++)
    {
        painter.drawLine((image->width()*i)/wgrads,0,(image->width()*i)/wgrads,image->height());
        painter.drawText((image->width()*i)/wgrads-15,15,tr("%1").arg(maxvalue*i/wgrads));
    }

    for(int i = 0 ; i <= hgrads ; i++)
    {
        painter.drawLine(0,(image->height()*(hgrads-i))/hgrads,image->width(),(image->height()*(hgrads-i))/hgrads);
        painter.drawText(0,(image->height()*(hgrads-i))/hgrads+15,QString("%1").arg((int)max*i/hgrads));
    }

    setPixmap(QPixmap::fromImage(*image)) ;
    showNormal() ;
    raise();

}

void HistogramLabel::saveToDisk()
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
