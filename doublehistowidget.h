#ifndef DoubleHistoWidget_H
#define DoubleHistoWidget_H

#include <QWidget>


class DoubleHistoWidget : public QWidget
{
    Q_OBJECT
QVector<double> &data;

QVector<int> class_count ;
int max_class_count;
double min,max;
public:
    explicit DoubleHistoWidget(QWidget *parent, QVector<double> &data);
    ~DoubleHistoWidget();
QSize sizeHint() const {return QSize(640,480);}
QSize minimumSizeHint() const {return QSize(640,480);}
int getMaxCount() {return max_class_count ;}
int getDataCount() {return data.count() ;}
int getClassCount() {return class_count.count() ;}
double getMin(){return min;}
double getMax(){return max;}

private:
QColor colorFor(float alpha);
double mean(int i);
protected:
virtual void paintEvent(QPaintEvent * event);
};

#endif // DoubleHistoWidget_H
