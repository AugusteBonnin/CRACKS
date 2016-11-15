#ifndef DOCFORM_H
#define DOCFORM_H

#include "form.h"

#include <QTextBrowser>



class DocForm : public Form
{
public:
    DocForm(MainWindow * mainWindow);
protected:
    QTextBrowser * browser ;
};

#endif // DOCFORM_H
