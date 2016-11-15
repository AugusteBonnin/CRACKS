#ifndef CROPZOOMWIDGET_H
#define CROPZOOMWIDGET_H

#include "cropquadwidget.h"

#include <QWidget>

class CropZoomWidget : public CropQuadWidget
{
    Q_OBJECT
public:
    explicit CropZoomWidget(Form *parent = 0);
    virtual QSize	sizeHint() const;
    void setZoomImage(QImage &image);
    virtual QSize minimumSizeHint() const;
    virtual QSize maximumSizeHint() const;
signals:

public slots:

    void updatePOI(QPoint p);
    void updateSelection(int s);
protected:
    void mouseMoveEvent(QMouseEvent *event);
private:

};

#endif // CROPZOOMWIDGET_H
