#include "docform.h"
#include "openingdocform.h"
#include "mainwindow.h"

OpeningDocForm::OpeningDocForm(MainWindow *parent) :
    DocForm(parent)
{
    browser->setSource(QUrl("doc/opening.html"));
}

OpeningDocForm::~OpeningDocForm()
{
}
