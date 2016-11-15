#ifndef ErosionWorker_H
#define ErosionWorker_H

#include <QObject>
#include <QRect>
#include <QRunnable>

class OpeningPage ;

class ErosionWorker : public QObject ,public QRunnable
{
    Q_OBJECT

    OpeningPage * openingForm ;
    QRect zone ;
public:
    ErosionWorker(OpeningPage *parent, QRect &zone);
    void run() ;
signals :
    void setProgressIncrement(int) ;
};

#endif // ErosionWorker_H
