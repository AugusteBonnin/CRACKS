#ifndef LogLogHistoResult_H
#define LogLogHistoResult_H

#include "histopage.h"
#include "historesult.h"

#include <QLabel>
#include <QVector>




class LogLogHistoResult : public HistoResult
{
    Q_OBJECT

    HistoPage * page ;

public:
    LogLogHistoResult(HistoPage *parent, QString name , QVector<double> &data, int index);
};

#endif // LogLogHistoResult_H
