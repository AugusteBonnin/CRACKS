#ifndef LogLogHistoWidget_H
#define LogLogHistoWidget_H

#include <QWidget>


class LogLogHistoWidget : public QWidget
{
    Q_OBJECT
QVector<double> &data;

QVector<int> class_count ;
int max_class_count;
double min,max;
public:
    explicit LogLogHistoWidget(QWidget *parent, QVector<double> &data);
    ~LogLogHistoWidget();
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

#endif // LogLogHistoWidget_H
