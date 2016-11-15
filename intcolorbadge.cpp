#include "intcolorbadge.h"

#include <QGridLayout>

IntColorBadge::IntColorBadge(QWidget *parent, QColor color, QString lbl)
    : ColorBadge(parent,color)
{
QGridLayout * layout = new QGridLayout;
setLayout(layout);
label = new QLabel(lbl,this);
QFont font = label->font();
font.setBold(true);
label->setFont(font);
label->setAlignment(Qt::AlignCenter);
layout->addWidget(label);
}
