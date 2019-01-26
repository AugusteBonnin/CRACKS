#ifndef IntRangedMapResult_H
#define IntRangedMapResult_H

#include "form.h"
#include "mapresult.h"

#include <QWidget>

namespace Ui {
class IntMapResult;
}

class IntRangedMapResult : public MapResult
{
    Q_OBJECT

    QVector<int> &data;
    QVector<QColor> colors;
    QList<int> class_value ;
public:
    explicit IntRangedMapResult(MapPage *parent, QString titre, QVector<int> &data, int index, int min, int max, int step, int type);
    ~IntRangedMapResult();

private:

    static QColor colorFor(float alpha);
protected:

    void paintEvent(QPaintEvent *e);
};

#endif // IntRangedMapResult_H
