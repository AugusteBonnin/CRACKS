#ifndef PHASE_H
#define PHASE_H

#include <QWidget>

class MainWindow ;

class Phase
{
public:
    Phase();
static QWidget * newFromPhase(uint phase, MainWindow *mainWindow) ;
signals:

public slots:
private:
static QWidget *createSimpleHistoPage(MainWindow *mainWindow);
static QWidget *createDynamicHistoPage(MainWindow *mainWindow);
static QWidget *createSimpleMapPage(MainWindow *mainWindow);
static QWidget *createDynamicMapPage(MainWindow *mainWindow);
};

#endif // PHASE_H
