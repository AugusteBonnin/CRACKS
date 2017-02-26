#ifndef HISTOPAGEFACTORIES_H
#define HISTOPAGEFACTORIES_H

#include "doublehistoresult.h"
#include "doublerangedhistoresult.h"
#include "histopage.h"
#include "inthistoresult.h"

class HistoPageFactory
{
public:
     virtual HistoPage * createHistoPage(MainWindow*) = 0 ;
};


class SimpleHistoPageFactory : public HistoPageFactory
{
public :
    HistoPage * createHistoPage(MainWindow*mainWindow);
};

class DynamicHistoPageFactory : public HistoPageFactory
{
public :
    HistoPage * createHistoPage(MainWindow*mainWindow);
};


#endif // HISTOPAGEFACTORIES_H
