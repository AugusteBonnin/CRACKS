#ifndef SCROLLABLEOPENGLWIDGET_H
#define SCROLLABLEOPENGLWIDGET_H

#include "form.h"
#include "kdtree2d.h"

#include <QMouseEvent>
#include <QOpenGLFunctions>
#include <QOpenGLWidget>

#define PROGRAM_VERTEX_ATTRIBUTE 0
#define PROGRAM_TEXCOORD_ATTRIBUTE 1
#define PROGRAM_COLOR_ATTRIBUTE 2

#include "customtypes.h"
#include <QSettings>

class ScrollableOpenGLWidget : public QOpenGLWidget , protected QOpenGLFunctions
{
Q_OBJECT

public:
    ScrollableOpenGLWidget(Form *parent);
    void resetImage(QImage &image);
    QSize sizeHint() const
    {
        return QSize(3000,3000);
    }
    QImage getImage();
    QImage getScreenshot();
    void resetToSavedState();
    void saveState();

    void resetToSize(QSize size);
signals :
    void ScaleChanged(double);
protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    void mouseReleaseEvent(QMouseEvent *);

    bool isDragging ;
    QPointF dragStart;
    QPointF & center;
    double & scale ;

    //QImage image ;

    MainWindow * mainWindow ;
    QSettings settings;
};

#endif // SCROLLABLEOPENGLWIDGET_H
