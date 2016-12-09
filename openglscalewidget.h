#ifndef OPENGLSCALEWIDGET_H
#define OPENGLSCALEWIDGET_H

#include <QWidget>

class OpenGLScaleWidget : public QWidget
{
    Q_OBJECT

    double scale;
    QSize imageSize;
public:
    explicit OpenGLScaleWidget(QWidget *parent , double s,QSize size);

signals:

public slots:
    void ScaleChanged(double arg);
protected:
    void paintEvent(QPaintEvent *paintEvent);
};

#endif // OPENGLSCALEWIDGET_H
