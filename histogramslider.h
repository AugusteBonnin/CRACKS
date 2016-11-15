#ifndef HISTOGRAMSLIDER_H
#define HISTOGRAMSLIDER_H

#include <QAbstractSlider>
#include "doubleimage.h"

class HistogramSlider : public QAbstractSlider
{
    Q_OBJECT
    DoubleImage *image;
    int *bins ;
    int max ;
    bool mouseDragged ;
    int last_width ;
public:
    ~HistogramSlider();

    HistogramSlider(QWidget * parent);
    void setImage(DoubleImage *image);

    QSize minimumSizeHint() const;
    QSize sizeHint() const;

    void computeBins();
signals:

public slots:
    void KMeans();

protected :
   void paintEvent(QPaintEvent *e);
   void mouseMoveEvent(QMouseEvent *e);
   void mousePressEvent(QMouseEvent *e);
   void mouseReleaseEvent(QMouseEvent *e);
};

#endif // HISTOGRAMSLIDER_H
