#ifndef PHASE_H
#define PHASE_H

#include "frame.h"

#include <QWidget>

class MainWindow ;

class Phase
{
public:
    Phase();
static Frame * newFromPhase(uint phase, MainWindow *mainWindow) ;
signals:

public slots:
private:
static Page *createSimpleHistoPage(MainWindow *mainWindow);
static Page *createDynamicHistoPage(MainWindow *mainWindow);
static Page *createSimpleMapPage(MainWindow *mainWindow);
static Page *createDynamicMapPage(MainWindow *mainWindow);
};

#endif // PHASE_H
