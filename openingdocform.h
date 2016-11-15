#ifndef OpeningDocForm_H
#define OpeningDocForm_H

#include "docform.h"
#include "form.h"

#include <QWidget>

class OpeningDocForm : public DocForm
{
    Q_OBJECT

public:
    explicit OpeningDocForm(MainWindow *parent);
    ~OpeningDocForm();

};

#endif // OpeningDocForm_H
