#ifndef LogLogHistoResult_H
#define LogLogHistoResult_H

#include "histopage.h"
#include "historesult.h"

#include <QLabel>
#include <QVector>




class LogLogHistoResult : public HistoResult
{
    Q_OBJECT

    int index;

    HistoPage * page ;


    QVector<QLabel*> horizontalLabels;
public:
    LogLogHistoResult(HistoPage *parent, QString name , QVector<double> &data, int index);

public slots:
    void setFont();
protected:
    void paintEvent(QPaintEvent *);
};

#endif // LogLogHistoResult_H
