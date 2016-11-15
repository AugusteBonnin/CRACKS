#include "cropdocform.h"
#include "croppage.h"
#include "cropparamform.h"
#include "mainwindow.h"
#include "ui_cropform.h"

#include <QDir>
#include <QPainter>
#include <QSettings>
#include <QTimer>

CropPage::CropPage(MainWindow *parent) :
    Page(parent)
{

    docForm = new CropDocForm(parent) ;
    paramForm = new CropParamForm(parent , this) ;


    QHBoxLayout * layout = new QHBoxLayout ;
    quadWidget = new CropQuadWidget;
    zoomWidget = new CropZoomWidget;
    resultWidget = new ImageWidget;

    layout->addWidget(quadWidget);
    layout->addWidget(zoomWidget);
    layout->addWidget(resultWidget);

    setLayout(layout);

    connect(quadWidget,SIGNAL(selectionChanged(int)),zoomWidget,SLOT(updateSelection(int)));
    connect(quadWidget,SIGNAL(POIchanged(QPoint)),zoomWidget,SLOT(updatePOI(QPoint)));
    connect(zoomWidget,SIGNAL(POIchanged(QPoint)),quadWidget,SLOT(updateCorner(QPoint)));

    connect(quadWidget,SIGNAL(quadChanged(QPolygon)),this,SLOT(remap(QPolygon)));
    connect(zoomWidget,SIGNAL(quadChanged(QPolygon)),this,SLOT(remap(QPolygon)));

    quadWidget->setFullImage(mainWindow->initialImage);
    quadWidget->initSourcePolygon();

    zoomWidget->setZoomImage(mainWindow->initialImage);
    zoomWidget->initSourcePolygon();

    remap(quadWidget->polygon);

    parent->action_next->setEnabled(true);

}

CropPage::~CropPage()
{
 }

void
CropPage::RAZ()
{
    quadWidget->RAZ();
    zoomWidget->RAZ();
    zoomWidget->setZoomImage(mainWindow->initialImage);

}

void CropPage::makeRectangle()
{
    quadWidget->makeRectangle();
    zoomWidget->setZoomImage(mainWindow->initialImage);
}

void CropPage::nextPhase()
{
    if (!settings.value("Apply0",QVariant(true)).toBool())
    {
        mainWindow->croppedImage = mainWindow->initialImage ;
    }

    if (settings.value("CropForm-SaveJPG",QVariant(true)).toBool())
{
        QString fileName = tr("Phase1-%1").arg(settings.value("File",QVariant(QDir::homePath())).toString());
   mainWindow->croppedImage.save(fileName) ;
}
}

void CropPage::prevPhase()
{

}

void CropPage::reinit()
{
    quadWidget->setFullImage(mainWindow->initialImage);
    quadWidget->initSourcePolygon();

    zoomWidget->setZoomImage(mainWindow->initialImage);
    zoomWidget->initSourcePolygon();

    remap(quadWidget->polygon);
}

void CropPage::initImages()
{


}

void CropPage::remap(QPolygon p)
{
    qreal largeur = QLineF(p[0],p[1]).length() ;
    largeur = qMin(largeur,QLineF(p[2],p[3]).length());

    qreal hauteur = QLineF(p[1],p[2]).length() ;
    hauteur = qMin(hauteur,QLineF(p[3],p[0]).length());

    QPolygon target ;
    target << QPoint (0,0) << QPoint(largeur,0) << QPoint(largeur,hauteur) << QPoint(0,hauteur);

    QImage & im1 = mainWindow->initialImage ;
    QImage & im2 = mainWindow->croppedImage ;
    im2 = QImage((int)largeur,(int)hauteur,QImage::Format_ARGB32);
    QTransform t ;
    QTransform::quadToQuad(p,target,t);

    QSettings settings;
    settings.setValue("CropForm-Polygon0",QVariant(p[0]));
    settings.setValue("CropForm-Polygon1",QVariant(p[1]));
    settings.setValue("CropForm-Polygon2",QVariant(p[2]));
    settings.setValue("CropForm-Polygon3",QVariant(p[3]));

    QPainter painter(&im2);
    painter.setTransform(t);
    painter.setRenderHint(QPainter::SmoothPixmapTransform,true);
    painter.drawImage(0,0,im1);
    resultWidget->setFullImage(im2);
    resultWidget->repaint();
}


