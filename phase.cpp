#include "contourpage.h"
#include "croppage.h"
#include "frame.h"
#include "histopage.h"
#include "histopagefactories.h"
#include "intropage.h"
#include "mappagefactory.h"
#include "openingpage.h"
#include "phase.h"
#include "regularizationpage.h"
#include "reportpage.h"
#include "roadspage.h"
#include "skelpage.h"

Phase::Phase()
{

}

Frame * Phase::newFromPhase(uint phase, MainWindow *mainWindow)
{
    Page * widget ;
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
        widget  = createSimpleMapPage(mainWindow);
        break ;
    case 9 :
        widget  = new ReportPage(mainWindow);
        break ;
    case 10 :
        widget = createDynamicHistoPage(mainWindow);
        break ;

    case 11 :
        widget = createDynamicMapPage(mainWindow);
        break ;

    case 12 :
        exit(0);
        break ;

     }

    return new Frame(mainWindow,widget) ;
}

Page * Phase::createSimpleHistoPage(MainWindow*mainWindow)
{
    SimpleHistoPageFactory factory;
    Page * widget  = factory.createHistoPage(mainWindow);
    return widget;
}

Page * Phase::createDynamicHistoPage(MainWindow*mainWindow)
{
    DynamicHistoPageFactory factory;
    Page * widget  = factory.createHistoPage(mainWindow);
    return widget;
}

Page *Phase::createSimpleMapPage(MainWindow *mainWindow)
{
    SimpleMapPageFactory factory;
    Page * widget = factory.createMapPage(mainWindow) ;
    return widget ;
}

Page *Phase::createDynamicMapPage(MainWindow *mainWindow)
{
    DynamicMapPageFactory factory;
    Page * widget = factory.createMapPage(mainWindow) ;
    return widget ;
}
