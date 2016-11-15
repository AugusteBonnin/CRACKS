#include "docform.h"
#include "cropdocform.h"
#include "mainwindow.h"

CropDocForm::CropDocForm(MainWindow *parent) :
    DocForm(parent)
{
    browser->setSource(QUrl("doc/recadrage.html"));
}

CropDocForm::~CropDocForm()
{
}
