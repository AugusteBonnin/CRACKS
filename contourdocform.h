#ifndef ContourDocForm_H
#define ContourDocForm_H

#include "docform.h"
#include "form.h"

#include <QWidget>

class ContourDocForm : public DocForm
{
    Q_OBJECT

public:
    explicit ContourDocForm(MainWindow *parent);
    ~ContourDocForm();

};

#endif // ContourDocForm_H
