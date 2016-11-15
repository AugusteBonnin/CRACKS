#ifndef INTHISTORESULT_H
#define INTHISTORESULT_H

#include "histopage.h"
#include "historesult.h"

#include <QLabel>
#include <QVector>


namespace Ui {
class IntHistoWidget;
}

class IntHistoResult : public HistoResult
{
    Q_OBJECT

    int index;

    HistoPage * page ;

    Ui::IntHistoWidget *ui;

    QVector<QLabel*> horizontalLabels;
public:
    IntHistoResult(HistoPage *parent,QString name , QVector<int> &data, int index);
    QSize sizeHint() const;
    QSize minimumSizeHint() const;
    QSize maximumSizeHint() const;
public slots:
    void setFont();
protected:
    void paintEvent(QPaintEvent *);
};

#endif // INTHISTORESULT_H
