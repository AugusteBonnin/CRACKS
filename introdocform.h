#ifndef INTRODOCFORM_H
#define INTRODOCFORM_H

#include "docform.h"
#include "form.h"

#include <QWidget>

class IntroDocForm : public DocForm
{
    Q_OBJECT

public:
    explicit IntroDocForm(MainWindow *parent);
    ~IntroDocForm();

};

#endif // INTRODOCFORM_H
