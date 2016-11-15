#ifndef DoubleMapResult_H
#define DoubleMapResult_H

#include "form.h"
#include "mapresult.h"

#include <QWidget>

namespace Ui {
class IntMapResult;
}

#define CLASS_COUNT 10

class DoubleMapResult : public MapResult
{
    Q_OBJECT

    QVector<double> &data;
    QVector<QColor> colors;
    QVector<double> classes_limits ;

    typedef struct Element{
        double value;
        double length;
        bool operator<(const Element & other) const {return value<other.value;}
    } Element;
public:
    explicit DoubleMapResult(MapPage *parent, QString titre, QVector<double> &data, int index, int type);
    ~DoubleMapResult();

private:

    static QColor colorFor(float alpha);
protected:

    void paintEvent(QPaintEvent *e);
};

#endif // INTMAPRESULT_H
