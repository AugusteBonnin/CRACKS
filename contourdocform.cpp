#include "docform.h"
#include "contourdocform.h"
#include "mainwindow.h"

ContourDocForm::ContourDocForm(MainWindow *parent) :
    DocForm(parent)
{
    browser->setUrl(tr("qrc:/docs/aide.html#Contour"));
}

ContourDocForm::~ContourDocForm()
{
}
