#include "docform.h"
#include "cropdocform.h"
#include "mainwindow.h"
#include <QApplication>
CropDocForm::CropDocForm(MainWindow *parent) :
    DocForm(parent)
{
    browser->setUrl(tr("qrc:/docs/aide.html#Recadrage"));
}

CropDocForm::~CropDocForm()
{
}
