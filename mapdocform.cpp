#include "mapdocform.h"

MapDocForm::MapDocForm(MainWindow *parent) :
    DocForm(parent)
{
    browser->setUrl(QUrl("file://"+QApplication::applicationDirPath()+tr("/doc/aide.html#Cartes")));
}

MapDocForm::~MapDocForm()
{
}
