#include "doubleimage.h"
#include "mainwindow.h"
#include "regularizationdocform.h"
#include "regularizationpage.h"
#include "regularizationparamform.h"
#include "regularizationworker.h"
#include "ui_regularizationform.h"

#include <QSettings>
#include <QThreadPool>
#include <QDir>
#include <QTimer>

RegularizationPage::RegularizationPage(MainWindow *parent) :
    Page(parent)
{
    docForm = new RegularizationDocForm(parent);

    paramForm = new RegularizationParamForm(parent,this);
    originalImage = mainWindow->croppedImage ;

    outputImage = new DoubleImage(originalImage.width(),originalImage.height());

    originalWidget = new ImageWidget;
    resultWidget = new ImageWidget;

    QHBoxLayout * layout = new QHBoxLayout;
    layout->addWidget(originalWidget);
    layout->addWidget(resultWidget);
    setLayout(layout);

    originalWidget->setFullImage(originalImage);
    resultWidget->setFullImage(originalImage);

    computed = false ;

    mainWindow->action_next->setEnabled(true);

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
        if (settings.value("RegularizationForm-SaveJPG",QVariant(true)).toBool())
    {
            QString fileName = tr("Phase2-%1").arg(settings.value("File",QVariant(QDir::homePath())).toString());
       mainWindow->croppedImage.save(fileName) ;
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


