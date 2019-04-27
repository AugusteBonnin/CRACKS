#include "frame.h"

#include <QGridLayout>

Frame::Frame(QWidget *parent,Page * page) : QWidget(parent)
{

    this->page = page ;

    QGridLayout * layout = new QGridLayout(this) ;

    HSplitter = new QSplitter(Qt::Horizontal,this);
    //HSplitter->restoreState(settings.value(QString("HSplitter-%1").arg(page->getTitle())).toByteArray());
    layout->addWidget(HSplitter);
    //connect(HSplitter,SIGNAL(splitterMoved(int,int)),this,SLOT(saveSplittersState()));
    VSplitter = new QSplitter(Qt::Vertical,this);
    //VSplitter->restoreState(settings.value(QString("VSplitter-%1").arg(page->getTitle())).toByteArray());
    HSplitter->addWidget(page->getParamForm());
    HSplitter->addWidget(VSplitter);
    VSplitter->addWidget(page->getDocForm());
    VSplitter->addWidget(page);
page->setSizePolicy(QSizePolicy(QSizePolicy::Ignored,QSizePolicy::Preferred));
    //connect(VSplitter,SIGNAL(splitterMoved(int,int)),this,SLOT(saveSplittersState()));
    QList<int> h;
    h << 128 << 768;
    QList<int> v;
    v << 128 << 512;
    HSplitter->setSizes(h);
    VSplitter->setSizes(v);

    setLayout(layout);
}

void Frame::saveSplittersState()
{
    settings.setValue(QString("HSplitter-%1").arg(page->getTitle()),HSplitter->saveState());
    settings.setValue(QString("VSplitter-%1").arg(page->getTitle()),VSplitter->saveState());
}
