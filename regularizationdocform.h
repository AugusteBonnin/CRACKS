#ifndef RegularizationDocForm_H
#define RegularizationDocForm_H

#include "docform.h"
#include "form.h"

#include <QWidget>

class RegularizationDocForm : public DocForm
{
    Q_OBJECT

public:
    explicit RegularizationDocForm(MainWindow *parent);
    ~RegularizationDocForm();

};

#endif // RegularizationDocForm_H
