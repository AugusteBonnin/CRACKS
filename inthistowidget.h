#ifndef INTHISTOWIDGET_H
#define INTHISTOWIDGET_H

#include <QWidget>


class IntHistoWidget : public QWidget
{
    Q_OBJECT
QVector<int> &data;
typedef struct Element{
    int value ;
    int index ;
    bool operator<(const Element & other) const {return value<other.value;}
} Element ;
QVector<Element> sorted ;
QVector<int> class_count ;
QVector<int> class_value ;
int max_class_count;
public:
    explicit IntHistoWidget(QWidget *parent, QVector<int> &data);
    ~IntHistoWidget();
int getMaxCount() {return max_class_count ;}
int getDataCount() {return data.count() ;}
int getClassCount() {return class_count.count() ;}
QVector<int> & getValues() {return class_value ;}
private:
QColor colorFor(float alpha);
protected:
virtual void paintEvent(QPaintEvent * event);
};

#endif // INTHISTOWIDGET_H
