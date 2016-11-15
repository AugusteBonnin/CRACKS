#ifndef COLORBADGE_H
#define COLORBADGE_H

#include <QWidget>



class ColorBadge : public QWidget
{
    Q_OBJECT

    QColor color;
public:
    ColorBadge(QWidget * parent,QColor color);
protected:
    void paintEvent(QPaintEvent *e);
};

#endif // COLORBADGE_H
