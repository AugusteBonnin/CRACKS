#include "docform.h"
#include "contourdocform.h"
#include "mainwindow.h"

ContourDocForm::ContourDocForm(MainWindow *parent) :
    DocForm(parent)
{
    browser->setUrl(QUrl("file://"+QApplication::applicationDirPath()+tr("/doc/aide.html#Contour")));
}

ContourDocForm::~ContourDocForm()
{
}
