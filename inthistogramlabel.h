#ifndef IntHistogramLabel_H
#define IntHistogramLabel_H

#include <QImage>
#include <QLabel>



class IntHistogramLabel : public QLabel
{
    Q_OBJECT

    QImage *image;
    int * bins ;
    int minvalue,maxvalue ;
    int bins_count ;
public:
    IntHistogramLabel(int w, int h, int minvalue, int maxvalue);
    ~IntHistogramLabel() ;
    void insertValue(int value);
    void saveToDisk();
    void display();
signals:

public slots:
};

#endif // IntHistogramLabel_H
