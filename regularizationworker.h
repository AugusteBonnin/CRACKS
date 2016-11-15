#ifndef REGULARIZATIONWORKER_H
#define REGULARIZATIONWORKER_H

#include <QObject>
#include <QRect>
#include <QRunnable>

class RegularizationPage ;

class RegularizationWorker : public QObject ,public QRunnable
{
    Q_OBJECT

    RegularizationPage * regularizationForm ;
    QRect zone ;
public:
    RegularizationWorker(RegularizationPage *parent, QRect &zone);
    void run() ;
signals :
    void setProgressIncrement(int) ;
};

#endif // REGULARIZATIONWORKER_H
