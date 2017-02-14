#include "docform.h"
#include "introdocform.h"
#include "mainwindow.h"
#include "ui_introdocform.h"
#include <QDir>
IntroDocForm::IntroDocForm(MainWindow *parent) :
    DocForm(parent)
{
    browser->setUrl (tr("qrc:/docs/aide.html#Introduction"));
}

IntroDocForm::~IntroDocForm()
{
}
