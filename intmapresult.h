#ifndef INTMAPRESULT_H
#define INTMAPRESULT_H

#include "form.h"
#include "mapresult.h"

#include <QWidget>

namespace Ui {
class IntMapResult;
}

class IntMapResult : public MapResult
{
    Q_OBJECT

    QVector<int> &data;
    QVector<QColor> colors;
    QList<int> class_value ;
public:
    explicit IntMapResult(MapPage *parent, QString titre, QVector<int> &data, int index, int type);
    ~IntMapResult();

private:

    static QColor colorFor(float alpha);
protected:

    void paintEvent(QPaintEvent *e);
};

#endif // INTMAPRESULT_H
