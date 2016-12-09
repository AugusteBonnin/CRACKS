#ifndef SCALEWIDGET_H
#define SCALEWIDGET_H

#include <QWidget>

class ScaleWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ScaleWidget(QWidget *parent);

signals:

public slots:

protected :
    void paintEvent(QPaintEvent *paintEvent);
};

#endif // SCALEWIDGET_H
