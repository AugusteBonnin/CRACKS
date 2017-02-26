#include "contourpage.h"
#include "croppage.h"
#include "histopage.h"
#include "histopagefactories.h"
#include "intropage.h"
#include "mappage.h"
#include "openingpage.h"
#include "phase.h"
#include "regularizationpage.h"
#include "reportpage.h"
#include "roadspage.h"
#include "skelpage.h"

Phase::Phase()
{

}

QWidget * Phase::newFromPhase(uint phase, MainWindow *mainWindow)
{
    QWidget * widget ;
    switch (phase)
    {
    case 0 :
        widget  = new IntroPage(mainWindow);
        break ;
    case 1 :
        //widget  = new IntroPage(mainWindow);
        widget  = new CropPage(mainWindow);
        break ;
    case 2 :
        widget  = new RegularizationPage(mainWindow);
        break ;
    case 3 :
        widget  = new OpeningPage(mainWindow);
        break ;
    case 4 :
        widget  = new ContourPage(mainWindow);
        break ;
    case 5 :
        widget  = new SkelPage(mainWindow);
        break ;
    case 6 :
        widget  = new RoadsPage(mainWindow);
        break ;
    case 7 :
        widget = createSimpleHistoPage(mainWindow) ;
        break ;
    case 8 :
        widget  = new MapPage(mainWindow);
        break ;
    case 9 :
        widget  = new ReportPage(mainWindow);
        break ;

    }

    return widget ;
}

QWidget * Phase::createSimpleHistoPage(MainWindow*mainWindow)
{
    SimpleHistoPageFactory factory;
    QWidget * widget  = factory.createHistoPage(mainWindow);
    return widget;
}
