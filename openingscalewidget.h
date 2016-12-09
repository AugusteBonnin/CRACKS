#ifndef OPENINGSCALEWIDGET_H
#define OPENINGSCALEWIDGET_H

#include <QWidget>

class OpeningScaleWidget : public QWidget
{
    Q_OBJECT
    QSize imageSize;
public:
    explicit OpeningScaleWidget(QWidget *parent, QSize size);

signals:

public slots:
protected:
    void paintEvent(QPaintEvent *paintEvent);
};

#endif // OPENINGSCALEWIDGET_H
