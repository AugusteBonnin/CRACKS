#include "dilatationworker.h"
#include "erosionworker.h"
#include "openingpage.h"
#include "openingparamform.h"
#include "scalewidget.h"
#include "ui_openingform.h"

#include <QDir>
#include <QSettings>
#include <QThreadPool>
#include <QTimer>

OpeningPage::OpeningPage(MainWindow *parent) :
    Page(parent)
{
    originalImage = new DoubleImage(*(mainWindow->regularizedImage)) ;
    originalImage->computeMinMax();
    originalQImage = QImage(originalImage->width(),originalImage->height(),QImage::Format_ARGB32);
    originalImage->toQImage(originalQImage);

    outputImage = new DoubleImage(originalImage->width(),originalImage->height());

    outputQImage = QImage(originalImage->width(),originalImage->height(),QImage::Format_ARGB32); ;

    minImage = new DoubleImage(originalImage->width(),originalImage->height());

    originalWidget = new ROIWidget(this);
    resultWidget = new ROIWidget(this);
    originalZoomWidget = new OpeningZoomWidget(this) ;
    resultZoomWidget = new OpeningZoomWidget(this) ;

    QHBoxLayout *layout = new QHBoxLayout;

    QVBoxLayout * vLayout1 = new QVBoxLayout;
    layout->addLayout(vLayout1);
    originalWidget->setFixedHeight(256);
    vLayout1->addWidget(originalWidget);
    ScaleWidget * sw1 = new ScaleWidget(this);
    sw1->setFixedHeight(24);
    vLayout1->addWidget(sw1);
    sw1->setVisible(settings.value("Crop/Unit",false).toBool());

    QVBoxLayout * vLayout = new QVBoxLayout;
    layout->addLayout(vLayout);
    originalZoomWidget->setFixedSize(256,256);
    vLayout->addWidget(originalZoomWidget);
    resultZoomWidget->setFixedSize(256,256);
    vLayout->addWidget(resultZoomWidget);
    scaleWidget = new OpeningScaleWidget(this,originalQImage.size());
    scaleWidget->setFixedHeight(40);
    vLayout->addWidget(scaleWidget);
    scaleWidget->setVisible(settings.value("Crop/Unit",false).toBool());

    QVBoxLayout * vLayout2 = new QVBoxLayout;
    layout->addLayout(vLayout2);
    resultWidget->setFixedHeight(256);
    vLayout2->addWidget(resultWidget);
    ScaleWidget * sw2 = new ScaleWidget(this);
    sw2->setFixedHeight(24);
    vLayout2->addWidget(sw2);
    sw2->setVisible(settings.value("Crop/Unit",false).toBool());

    setLayout(layout);


    connect(originalWidget,SIGNAL(ROIChanged(QRect)),originalZoomWidget,SLOT(setROI(QRect)));
    connect(originalWidget,SIGNAL(ROIChanged(QRect)),resultZoomWidget,SLOT(setROI(QRect)));
    connect(originalWidget,SIGNAL(ROIChanged(QRect)),resultWidget,SLOT(setROI(QRect)));
    connect(originalWidget,SIGNAL(mouseReleased()),this,SLOT(preview()));

    connect(resultWidget,SIGNAL(ROIChanged(QRect)),originalWidget,SLOT(setROI(QRect)));
    connect(resultWidget,SIGNAL(ROIChanged(QRect)),originalZoomWidget,SLOT(setROI(QRect)));
    connect(resultWidget,SIGNAL(ROIChanged(QRect)),resultZoomWidget,SLOT(setROI(QRect)));
    connect(resultWidget,SIGNAL(mouseReleased()),this,SLOT(preview()));

    connect(originalZoomWidget,SIGNAL(ROIChanged(QRect)),originalWidget,SLOT(setROI(QRect)));
    connect(originalZoomWidget,SIGNAL(ROIChanged(QRect)),resultZoomWidget,SLOT(setROI(QRect)));
    connect(originalZoomWidget,SIGNAL(ROIChanged(QRect)),resultWidget,SLOT(setROI(QRect)));
    connect(originalZoomWidget,SIGNAL(mouseReleased()),this,SLOT(preview()));

    connect(resultZoomWidget,SIGNAL(ROIChanged(QRect)),originalWidget,SLOT(setROI(QRect)));
    connect(resultZoomWidget,SIGNAL(ROIChanged(QRect)),originalZoomWidget,SLOT(setROI(QRect)));
    connect(resultZoomWidget,SIGNAL(ROIChanged(QRect)),resultWidget,SLOT(setROI(QRect)));
    connect(resultZoomWidget,SIGNAL(mouseReleased()),this,SLOT(preview()));

    paramForm = new OpeningParamForm(parent,this);
    docForm = new DocForm(parent,tr("qrc:/docs/aide.html#Ouverture"));

    initDone = false ;

    QTimer::singleShot(0,this,SLOT(initImages())) ;
}

OpeningPage::~OpeningPage()
{
}

void OpeningPage::nextPhase()
{
    if (!settings.value("OpeningParamForm-Apply",QVariant(true)).toBool())
    {
        if ((mainWindow->openedImage)&&(mainWindow->openedImage != mainWindow->regularizedImage))
        {
            delete mainWindow->openedImage ;
        }

        mainWindow->openedImage = new DoubleImage(*(mainWindow->regularizedImage)) ;
    }
    else
    {
        if (!computed)
        {
            compute_clicked();
        }
        if (settings.value("OpeningParamForm-SaveJPG",QVariant(true)).toBool())
        {
            mainWindow->trySaveDoubleImage(tr("Ouverture-"),mainWindow->openedImage);
        }
    }
}

void OpeningPage::reinit()
{
    originalImage = mainWindow->regularizedImage ;
    originalQImage = QImage(originalImage->width(),originalImage->height(),QImage::Format_ARGB32);
    originalImage->toQImage(originalQImage);

    delete outputImage ;
    outputImage = new DoubleImage(originalImage->width(),originalImage->height());

    outputQImage = QImage(originalImage->width(),originalImage->height(),QImage::Format_ARGB32); ;

    delete minImage ;
    minImage = new DoubleImage(originalImage->width(),originalImage->height());

    initImages();
}

void OpeningPage::invert_clicked()
{
    originalImage->invert();
    originalImage->computeMinMax();
    //originalImage->toQImage(outputQImage);
    originalImage->toQImage(originalQImage);
    originalWidget->setFullImage(originalQImage);
    //resultWidget->setFullImage(outputQImage);
    originalZoomWidget->setFullImage(originalQImage);
    //resultZoomWidget->setFullImage(outputQImage);
    preview();
}


void OpeningPage::initImages()
{
    if (settings.value("OpeningParamForm-Invert",false).toBool() )
        invert_clicked();

    originalWidget->setFullImage(originalQImage);
    resultWidget->setFullImage(originalQImage);
    originalZoomWidget->setFullImage(originalQImage);
    resultZoomWidget->setFullImage(originalQImage);
    int x1 = (originalQImage.width()>>1) ;
    int y1 = (originalQImage.height()>>1) ;
    int w2 = (originalZoomWidget->width()>>1)/settings.value("OpeningParamForm-Zoom",QVariant(1)).toInt() ;
    int h2 = (originalZoomWidget->height()>>1)/settings.value("OpeningParamForm-Zoom",QVariant(1)).toInt() ;
    QRect ROI(x1-w2,y1-h2,w2<<1,h2<<1);
    originalWidget->setROI(ROI);
    resultWidget->setROI(ROI);
    originalZoomWidget->setROI(ROI);
    resultZoomWidget->setROI(ROI);
    originalZoomWidget->setMagnification(settings.value("OpeningParamForm-Zoom",QVariant(1)).toInt());
    resultZoomWidget->setMagnification(settings.value("OpeningParamForm-Zoom",QVariant(1)).toInt());

    preview() ;

mainWindow->setActionsEnabled(true)
        ;
    computed = false ;
    initDone = true ;

}

void OpeningPage::compute_clicked()
{
    paramForm->setEnabled(false );

    QThreadPool * threadPool = QThreadPool::globalInstance() ;

    mainWindow->progress->setMinimum(0);
    mainWindow->progress->setValue(0);
    mainWindow->progress->setMaximum(2*originalImage->width()*originalImage->height());


    for (int i = 0 , y = 0 ; i < threadPool->maxThreadCount() ; i++)
    {
        int y2 = originalImage->height()*(i+1)/threadPool->maxThreadCount() ;
        QRect zone(0,y,originalImage->width(),y2-y) ;
        ErosionWorker * worker = new ErosionWorker(this,zone);
        connect(worker,SIGNAL(setProgressIncrement(int)),mainWindow,SLOT(receiveProgressIncrement(int)));
        threadPool->start(worker);
        y = y2 ;
    }

    while (threadPool->activeThreadCount())
    {
        qApp->processEvents();
        QThread::yieldCurrentThread();
    }

    for (int i = 0 , y = 0 ; i < threadPool->maxThreadCount() ; i++)
    {
        int y2 = originalImage->height()*(i+1)/threadPool->maxThreadCount() ;
        QRect zone(0,y,originalImage->width(),y2-y) ;
        DilatationWorker * worker = new DilatationWorker(this,zone);
        connect(worker,SIGNAL(setProgressIncrement(int)),mainWindow,SLOT(receiveProgressIncrement(int)));
        threadPool->start(worker);
        y = y2 ;
    }

    while (threadPool->activeThreadCount())
    {
        qApp->processEvents();
        QThread::yieldCurrentThread();
    }

    outputImage->computeMinMax();
    outputImage->toQImage(outputQImage);

    resultWidget->setImage(outputQImage);
    resultZoomWidget->setImage(outputQImage);

    mainWindow->openedImage = outputImage ;
    mainWindow->openedQImage = outputQImage ;

    paramForm->setEnabled(true);

    computed = true ;

    repaint() ;
}

void OpeningPage::preview()
{
    paramForm->setEnabled(false);

    QThreadPool * threadPool = QThreadPool::globalInstance() ;

    QRect ROI = originalWidget->getROI() ;

    mainWindow->progress->setMinimum(0);
    mainWindow->progress->setValue(0);
    mainWindow->progress->setMaximum(2*ROI.width()*ROI.height());


    for (int i = 0 , y = ROI.y() ; i < threadPool->maxThreadCount() ; i++)
    {
        int y2 = ROI.y()+ROI.height()*(i+1)/threadPool->maxThreadCount() ;
        QRect zone(ROI.x(),y,ROI.width(),y2-y) ;
        ErosionWorker * worker = new ErosionWorker(this,zone);
        connect(worker,SIGNAL(setProgressIncrement(int)),mainWindow,SLOT(receiveProgressIncrement(int)));
        threadPool->start(worker);
        y = y2 ;
    }

    while (threadPool->activeThreadCount())
    {
        qApp->processEvents();
        QThread::yieldCurrentThread();
    }

    for (int i = 0 , y = ROI.y() ; i < threadPool->maxThreadCount() ; i++)
    {
        int y2 = ROI.y()+ROI.height()*(i+1)/threadPool->maxThreadCount() ;
        QRect zone(ROI.x(),y,ROI.width(),y2-y) ;
        DilatationWorker * worker = new DilatationWorker(this,zone);
        connect(worker,SIGNAL(setProgressIncrement(int)),mainWindow,SLOT(receiveProgressIncrement(int)));
        threadPool->start(worker);
        y = y2 ;
    }

    while (threadPool->activeThreadCount())
    {
        qApp->processEvents();
        QThread::yieldCurrentThread();
    }

    outputImage->toQImage(outputQImage);

    resultWidget->setImage(outputQImage);
    resultZoomWidget->setImage(outputQImage);

    paramForm->setEnabled(true);

    repaint() ;
}



