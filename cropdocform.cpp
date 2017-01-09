#include "docform.h"
#include "cropdocform.h"
#include "mainwindow.h"
#include <QApplication>
CropDocForm::CropDocForm(MainWindow *parent) :
    DocForm(parent)
{
    browser->setUrl(QUrl("file://"+QApplication::applicationDirPath()+tr("/doc/aide.html#Recadrage")));
}

CropDocForm::~CropDocForm()
{
}
