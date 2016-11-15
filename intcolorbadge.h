#ifndef INTCOLORBADGE_H
#define INTCOLORBADGE_H

#include "colorbadge.h"

#include <QLabel>
#include <QObject>
#include <QWidget>

class IntColorBadge : public ColorBadge
{
    Q_OBJECT

    QLabel *label ;
public:
    IntColorBadge(QWidget * parent,QColor color,QString lbl);
};

#endif // INTCOLORBADGE_H
