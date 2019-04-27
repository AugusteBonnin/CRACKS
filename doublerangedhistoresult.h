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

    HistoPage * page ;
Ui::DoubleRangedHistoResult *ui;
public:
    explicit DoubleRangedHistoResult(HistoPage *parent, QString name , QVector<double> &data, int index, double min, double max);
    ~DoubleRangedHistoResult();

};

#endif // DOUBLERANGEDHISTORESULT_H
