#ifndef CropDocForm_H
#define CropDocForm_H

#include "docform.h"
#include "form.h"

#include <QWidget>

class CropDocForm : public DocForm
{
    Q_OBJECT

public:
    explicit CropDocForm(MainWindow *parent);
    ~CropDocForm();

};

#endif // CropDocForm_H
