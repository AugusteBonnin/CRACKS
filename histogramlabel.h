#ifndef HISTOGRAMLABEL_H
#define HISTOGRAMLABEL_H

#include <QImage>
#include <QLabel>



class HistogramLabel : public QLabel
{
    Q_OBJECT

    QImage *image;
    int * bins ;
    int maxvalue ;
    int step ;
    int wgrads ;
    int hgrads ;
    int bins_count ;
public:
    HistogramLabel(int w, int h, int maxvalue, int step, int wgrads, int hgrads);
    ~HistogramLabel() ;
    void insertValue(double value);
    void saveToDisk();
    void display();
signals:

public slots:
};

#endif // HISTOGRAMLABEL_H
