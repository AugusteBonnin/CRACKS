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
};

#endif // PHASE_H
