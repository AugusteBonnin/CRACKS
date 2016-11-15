#ifndef DilatationWorker_H
#define DilatationWorker_H

#include <QObject>
#include <QRect>
#include <QRunnable>

class OpeningPage ;

class DilatationWorker : public QObject ,public QRunnable
{
    Q_OBJECT

    OpeningPage * openingForm ;
    QRect zone ;
public:
    DilatationWorker(OpeningPage *parent, QRect &zone);
    void run() ;
signals :
    void setProgressIncrement(int) ;
};

#endif // DilatationWorker_H
