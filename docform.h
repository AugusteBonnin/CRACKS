#ifndef DOCFORM_H
#define DOCFORM_H

#include "form.h"

#include <QApplication>
#include <QTextBrowser>



class DocForm : public QTextBrowser
{
public:
    DocForm(QWidget * parent, QString url);
protected:
    QTextBrowser * browser ;
};

#endif // DOCFORM_H
