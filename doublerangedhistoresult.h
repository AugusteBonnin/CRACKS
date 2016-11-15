#ifndef DOUBLERANGEDHISTORESULT_H
#define DOUBLERANGEDHISTORESULT_H

#include "histopage.h"
#include "historesult.h"

#include <QWidget>

namespace Ui {
class DoubleRangedHistoResult;
}

class DoubleRangedHistoResult : public HistoResult
{
    Q_OBJECT
    int index;

    HistoPage * page ;

public:
    explicit DoubleRangedHistoResult(HistoPage *parent, QString name , QVector<double> &data, int index, double min, double max);
    ~DoubleRangedHistoResult();
protected:
    void paintEvent(QPaintEvent *e);
private:
    Ui::DoubleRangedHistoResult *ui;
    void setFont();
};

#endif // DOUBLERANGEDHISTORESULT_H
