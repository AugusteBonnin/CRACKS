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

    HistoPage * page ;

    Ui::DoubleRangedHistoResult *ui;

public:
    DoubleHistoResult(HistoPage *parent, QString name , QVector<double> &data, int index);


};

#endif // DoubleHistoResult_H
