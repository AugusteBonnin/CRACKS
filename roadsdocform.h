#ifndef RoadsDocForm_H
#define RoadsDocForm_H

#include "docform.h"
#include "form.h"

#include <QWidget>

class RoadsDocForm : public DocForm
{
    Q_OBJECT

public:
    explicit RoadsDocForm(MainWindow *parent);
    ~RoadsDocForm();

};

#endif // RoadsDocForm_H
