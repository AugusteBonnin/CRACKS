#include "docform.h"
#include "contourdocform.h"
#include "mainwindow.h"

ContourDocForm::ContourDocForm(MainWindow *parent) :
    DocForm(parent)
{
    browser->setSource(QUrl("doc/contour.html"));
}

ContourDocForm::~ContourDocForm()
{
}
