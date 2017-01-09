#ifndef DOCFORM_H
#define DOCFORM_H

#include "form.h"

#include <QWebEngineView>
#include <QApplication>



class DocForm : public Form
{
public:
    DocForm(MainWindow * mainWindow);
protected:
    QWebEngineView * browser ;
};

#endif // DOCFORM_H
