#include "form.h"
#include "mainwindow.h"

Form::Form(MainWindow *parent) : QWidget(parent)
{
mainWindow = parent ;
mainWindow->setActionsEnabled(false);
}

