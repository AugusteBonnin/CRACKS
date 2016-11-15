#include "mapdocform.h"

MapDocForm::MapDocForm(MainWindow *parent) :
    DocForm(parent)
{
    browser->setSource(QUrl("doc/opening.html"));
}

MapDocForm::~MapDocForm()
{
}
