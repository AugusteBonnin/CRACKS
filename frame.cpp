#include "frame.h"

#include <QGridLayout>

Frame::Frame(QWidget *parent,Page * page) : QWidget(parent)
{

    this->page = page ;

    QGridLayout * layout = new QGridLayout(this) ;

    HSplitter = new QSplitter(Qt::Horizontal,this);
    HSplitter->restoreState(settings.value(QString("HSplitter-%1").arg(page->getTitle())).toByteArray());
    layout->addWidget(HSplitter);
    connect(HSplitter,SIGNAL(splitterMoved(int,int)),this,SLOT(saveSplittersState()));
    VSplitter = new QSplitter(Qt::Vertical,this);
    VSplitter->restoreState(settings.value(QString("VSplitter-%1").arg(page->getTitle())).toByteArray());
    HSplitter->addWidget(page->getParamForm());
    HSplitter->addWidget(VSplitter);
    VSplitter->addWidget(page->getDocForm());
    VSplitter->addWidget(page);

    connect(VSplitter,SIGNAL(splitterMoved(int,int)),this,SLOT(saveSplittersState()));

    setLayout(layout);
}

void Frame::saveSplittersState()
{
    settings.setValue(QString("HSplitter-%1").arg(page->getTitle()),HSplitter->saveState());
    settings.setValue(QString("VSplitter-%1").arg(page->getTitle()),VSplitter->saveState());
}
