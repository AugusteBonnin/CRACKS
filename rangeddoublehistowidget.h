#ifndef RangedDoubleHistoWidget_H
#define RangedDoubleHistoWidget_H

#include <QWidget>


class RangedDoubleHistoWidget : public QWidget
{
    Q_OBJECT
QVector<double> &data;

QVector<int> class_count ;
int max_class_count;
double min,max;
public:
    explicit RangedDoubleHistoWidget(QWidget *parent, QVector<double> &data,double min,double max);
    ~RangedDoubleHistoWidget();
int getMaxCount() {return max_class_count ;}
int getDataCount() {return data.count() ;}
int getClassCount() {return class_count.count() ;}
double getMin(){return min;}
double getMax(){return max;}

private:
static QColor colorFor(float alpha);

protected:
virtual void paintEvent(QPaintEvent * event);
};

#endif // RangedDoubleHistoWidget_H
