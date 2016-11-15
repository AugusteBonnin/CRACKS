#include "docform.h"
#include "roadsdocform.h"
#include "mainwindow.h"

RoadsDocForm::RoadsDocForm(MainWindow *parent) :
    DocForm(parent)
{
    browser->setSource(QUrl("doc/roads.html"));
}

RoadsDocForm::~RoadsDocForm()
{
}
