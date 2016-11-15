#ifndef DYNAMICALANALYSIS_H
#define DYNAMICALANALYSIS_H

#include "doubleimage.h"

#include <QDir>
#include <QImage>
#include <QStringList>



class DynamicalAnalysis
{

    void phase1();
    void phase2() ;
    void phase3() ;

    QDir dir ;
    QStringList files ;
    int counter ;

    QImage initialImage ;
    DoubleImage * resultImage ;

    QVector<uint> birthDates ;
    QVector<uint> deathDates;
public:
    DynamicalAnalysis();
};

#endif // DYNAMICALANALYSIS_H
