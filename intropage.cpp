#include "imagewidget.h"
#include "intropage.h"
#include "introparamform.h"
#include "mainwindow.h"
#include "ui_introform.h"

#include <QSettings>
#include <QFileDialog>
#include <QStatusBar>
#include <QTimer>

IntroPage::IntroPage(MainWindow *parent) :
    Page(parent)
{
    widget = new ImageWidget(this);
    QGridLayout * layout = new QGridLayout;
    layout->addWidget(widget);
    setLayout(layout) ;

    paramForm = new IntroParamForm(parent,this) ;
    docForm = new IntroDocForm(parent) ;

parent->action_next->setEnabled(true);


}

IntroPage::~IntroPage()
{
 }

void IntroPage::setFile(QString file)
{
    image.load(file) ;
    widget->setFullImage(image);
}

void IntroPage::nextPhase()
{
    mainWindow->initialImage = image;
    settings.setValue("Size",image.size()) ;
}

void IntroPage::prevPhase()
{

}

void IntroPage::reinit()
{
    QString imagePath = settings.value("File").toString() ;

    setFile(imagePath);

}


void IntroPage::setImage(QImage &image)
{
    this->image = image ;

    QTimer::singleShot(100,this,SLOT(setWidgetImage()));
}

void IntroPage::setWidgetImage()
{
    widget->setFullImage(image);
    widget->repaint();
   }
