#include "imagewidget.h"
#include "form.h"

#include <QPainter>
#include <qevent.h>

ImageWidget::ImageWidget(Form *parent) : QWidget(parent)
{
form = parent ;
}

ImageWidget::~ImageWidget()
{
}

QSize ImageWidget::sizeHint() const
{
    return QSize(1024,1024);
}

void ImageWidget::paintEvent(QPaintEvent *event)
{
QPainter painter(this);
painter.drawRect(QRect(0,0,width()-1,height()-1));
painter.setTransform(imageToScreen);
painter.drawImage(0,0,background);
painter.drawRect(QRect(0,0,background.width()-1,background.height()-1));
}

void ImageWidget::setFullImage(QImage & image)
{
    background = image;
    if (image.width()/(float)image.height()<width()/(float)height())
    {
        qreal scale = height()/(float)image.height() ;
    imageToScreen = QTransform(scale,0,0,scale,(width()-image.width()*height()/(float)image.height())*.5,0);

    }
    else
    {
        qreal scale = width()/(float)image.width() ;
      imageToScreen = QTransform(scale,0,0,scale,0,(height()-image.height()*width()/(float)image.width())*.5);
    }
    screenToImage = imageToScreen.inverted() ;
}

void ImageWidget::setImage(QImage &image)
{
    background = image;
}


void ImageWidget::resizeEvent(QResizeEvent *event)
{
    if (background.width()/(float)background.height()<width()/(float)height())
    {
        qreal scale = height()/(float)background.height() ;
    imageToScreen = QTransform(scale,0,0,scale,(width()-background.width()*height()/(float)background.height())*.5,0);

    }
    else
    {
        qreal scale = width()/(float)background.width() ;
      imageToScreen = QTransform(scale,0,0,scale,0,(height()-background.height()*width()/(float)background.width())*.5);
    }
    screenToImage = imageToScreen.inverted() ;
}
