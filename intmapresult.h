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

    QVector<uint32_t> &data;
    QVector<QColor> colors;
    QList<int> class_value ;
public:
    explicit IntMapResult(MapPage *parent, QString titre, QVector<uint32_t> &data, int index, int type);
    ~IntMapResult();

private:

    static QColor colorFor(float alpha);
protected:

    void paintEvent(QPaintEvent *e);
};

#endif // INTMAPRESULT_H
