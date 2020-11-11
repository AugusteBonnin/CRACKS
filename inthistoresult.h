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

    HistoPage * page ;

    Ui::IntHistoWidget *ui;

public:
    IntHistoResult(HistoPage *parent,QString name , QVector<uint32_t> &data, uint32_t index);


};

#endif // INTHISTORESULT_H
