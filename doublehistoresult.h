#ifndef DoubleHistoResult_H
#define DoubleHistoResult_H

#include "histopage.h"
#include "historesult.h"

#include <QLabel>
#include <QVector>


namespace Ui {
class DoubleRangedHistoResult;
}

class DoubleHistoResult : public HistoResult
{
    Q_OBJECT

    int index;

    HistoPage * page ;

    Ui::DoubleRangedHistoResult *ui;

    QVector<QLabel*> horizontalLabels;
public:
    DoubleHistoResult(HistoPage *parent, QString name , QVector<double> &data, int index);

public slots:
    void setFont();
protected:
    void paintEvent(QPaintEvent *);
};

#endif // DoubleHistoResult_H
