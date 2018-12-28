#include "doubleimage.h"
#include "mainwindow.h"
#include "regularizationpage.h"
#include "regularizationparamform.h"
#include "regularizationworker.h"
#include "scalewidget.h"
#include "ui_regularizationform.h"

#include <QSettings>
#include <QThreadPool>
#include <QDir>
#include <QTimer>

RegularizationPage::RegularizationPage(MainWindow *parent) :
    Page(parent)
{
    docForm = new DocForm(parent,tr("qrc:/docs/aide.html#Regularisation"));

    paramForm = new RegularizationParamForm(parent,this);
    originalImage = mainWindow->croppedImage ;

    outputImage = new DoubleImage(originalImage.width(),originalImage.height());

    originalWidget = new ImageWidget;
    resultWidget = new ImageWidget;

    QHBoxLayout * layout = new QHBoxLayout;
    QVBoxLayout *vl1 = new QVBoxLayout;
    layout->addLayout((vl1));
    vl1->addWidget(originalWidget);
    originalWidget->setFullImage(originalImage);
    ScaleWidget *sw1 = new ScaleWidget(this) ;
    sw1->setFixedHeight(24);
    vl1->addWidget(sw1);
    sw1->setVisible(settings.value("Crop/Unit",false).toBool());
    QVBoxLayout *vl2 = new QVBoxLayout;
    layout->addLayout((vl2));
    vl2->addWidget(resultWidget);
    resultWidget->setFullImage(originalImage);
    ScaleWidget *sw2 = new ScaleWidget(this) ;
    sw2->setFixedHeight(24);
    vl2->addWidget(sw2);
    sw2->setVisible(settings.value("Crop/Unit",false).toBool());
    setLayout(layout);

    initDone = true ;
    computed = false ;

    parent->setActionsEnabled(true);

}

RegularizationPage::~RegularizationPage()
{

    delete outputImage ;

    if (mainWindow->regularizedImage)
        delete mainWindow->regularizedImage ;
    mainWindow->regularizedImage = NULL ;
}

void RegularizationPage::nextPhase()
{
    if (!settings.value("Apply1",QVariant(true)).toBool())
    {
        if (mainWindow->regularizedImage)
        {
            delete mainWindow->regularizedImage ;
        }

        mainWindow->regularizedImage = new DoubleImage(mainWindow->croppedImage) ;
    }
    else
    {
        if (!computed)
        {
            on_compute_clicked();
        }
        if (settings.value("RegularizationForm-SaveJPG",false).toBool())
    {
            mainWindow->trySaveDoubleImage(tr("Régularisation-"),mainWindow->regularizedImage);
    }
    }
}

void RegularizationPage::prevPhase()
{

}

void RegularizationPage::reinit()
{
    originalImage = mainWindow->croppedImage ;

    delete outputImage ;
    outputImage = new DoubleImage(originalImage.width(),originalImage.height());

    originalWidget->setFullImage(originalImage);
    resultWidget->setFullImage(originalImage);

    initDone = true ;
    computed = false ;
}

void RegularizationPage::on_compute_clicked()
{
    computed = true ;

    QThreadPool * threadPool = QThreadPool::globalInstance() ;

    mainWindow->progress->setMinimum(0);
    mainWindow->progress->setValue(0);
    mainWindow->progress->setMaximum(2*originalImage.width()*originalImage.height());


    for (int i = 0 , y = 0 ; i < threadPool->maxThreadCount() ; i++)
    {
        int y2 = originalImage.height()*(i+1)/threadPool->maxThreadCount() ;
        QRect zone(0,y,originalImage.width(),y2-y) ;
        RegularizationWorker * worker = new RegularizationWorker(this,zone);
        connect(worker,SIGNAL(setProgressIncrement(int)),mainWindow,SLOT(receiveProgressIncrement(int)));
        threadPool->start(worker);
        y = y2 ;
    }

    while (threadPool->activeThreadCount())
    {
        qApp->processEvents();
        QThread::yieldCurrentThread();
    }

    QImage resultImage(originalImage) ;
    DoubleImage *result = new DoubleImage(originalImage);
    DoubleImage * sub = result->substraction(outputImage) ;
    delete result ;
    sub->computeMinMax();
    sub->toQImage(resultImage);

    if (mainWindow->regularizedImage)
        delete mainWindow->regularizedImage ;
    mainWindow->regularizedImage = sub ;

    resultWidget->setFullImage(resultImage);


    repaint() ;
}


