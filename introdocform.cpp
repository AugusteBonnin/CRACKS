#include "docform.h"
#include "introdocform.h"
#include "mainwindow.h"
#include "ui_introdocform.h"

IntroDocForm::IntroDocForm(MainWindow *parent) :
    DocForm(parent)
{
    browser->setSource(QUrl("doc/introduction.html"));
}

IntroDocForm::~IntroDocForm()
{
}
