#-------------------------------------------------
#
# Project created by QtCreator 2016-03-31T15:32:10
#
#-------------------------------------------------

QT       += core gui svg

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CRACKS!

TEMPLATE = app

LIBS += -L/usr/local/lib  -lCGAL  -lCGAL_Core -lgmp -lmpfr
LIBS += -L/usr/lib/x86_64-linux-gnu/ -lboost_thread -lboost_system -lz
LIBS += -L . -lshp

INCLUDEPATH += /usr/local/include

SOURCES += main.cpp\
        mainwindow.cpp \
    statsform.cpp \
    phase.cpp \
    cropzoomwidget.cpp \
    cropquadwidget.cpp \
    regularizationworker.cpp \
    doubleimage.cpp \
    gaussianfilter.cpp \
    imagewidget.cpp \
    roiwidget.cpp \
    openingzoomwidget.cpp \
    erosionworker.cpp \
    dilatationworker.cpp \
    histogramslider.cpp \
    form.cpp \
    scrollablewidget.cpp \
    contourwidget.cpp \
    scrollableopenglwidget.cpp \
    skelwidget.cpp \
    roadswidget.cpp \
    histogramlabel.cpp \
    inthistogramlabel.cpp \
    kdtree2d.cpp \
    dynamicalanalysis.cpp \
    batchprocessing.cpp \
    introdocform.cpp \
    introparamform.cpp \
    docform.cpp \
    paramform.cpp \
    page.cpp \
    intropage.cpp \
    cropparamform.cpp \
    croppage.cpp \
    cropdocform.cpp \
    regularizationparamform.cpp \
    regularizationpage.cpp \
    regularizationdocform.cpp \
    openingdocform.cpp \
    openingparamform.cpp \
    openingpage.cpp \
    contourdocform.cpp \
    contourparamform.cpp \
    contourpage.cpp \
    skeldocform.cpp \
    skelparamform.cpp \
    skelpage.cpp \
    roadsdocform.cpp \
    roadsparamform.cpp \
    roadspage.cpp \
    histoparamform.cpp \
    cartoparamform.cpp \
    histopage.cpp \
    histodocform.cpp \
    result.cpp \
    historesult.cpp \
    inthistoresult.cpp \
    inthistowidget.cpp \
    skelworker.cpp \
    doublehistoresult.cpp \
    doublehistowidget.cpp \
    rangeddoublehistowidget.cpp \
    doublerangedhistoresult.cpp \
    mapparamform.cpp \
    mapdocform.cpp \
    intmapresult.cpp \
    colorbadge.cpp \
    intcolorbadge.cpp \
    mapwidget.cpp \
    junctionmapwidget.cpp \
    mappage.cpp \
    intrangedmapresult.cpp \
    doublemapresult.cpp \
    roadmapwidget.cpp \
    mapresult.cpp \
    saveform.cpp \
    reportpage.cpp \
    reportparamform.cpp \
    scalewidget.cpp \
    openingscalewidget.cpp \
    openglscalewidget.cpp

HEADERS  += mainwindow.h \
    statsform.h \
    phase.h \
    cropzoomwidget.h \
    cropquadwidget.h \
    regularizationworker.h \
    doubleimage.h \
    gaussianfilter.h \
    imagewidget.h \
    roiwidget.h \
    openingzoomwidget.h \
    erosionworker.h \
    dilatationworker.h \
    histogramslider.h \
    form.h \
    scrollablewidget.h \
    contourwidget.h \
    scrollableopenglwidget.h \
    skelwidget.h \
    customtypes.h \
    roadswidget.h \
    histogramlabel.h \
    inthistogramlabel.h \
    nanoflann.hpp \
    kdtree2d.h \
    dynamicalanalysis.h \
    batchprocessing.h \
    introdocform.h \
    introparamform.h \
    docform.h \
    paramform.h \
    page.h \
    intropage.h \
    cropparamform.h \
    croppage.h \
    cropdocform.h \
    regularizationparamform.h \
    regularizationpage.h \
    regularizationdocform.h \
    openingdocform.h \
    openingparamform.h \
    openingpage.h \
    contourdocform.h \
    contourparamform.h \
    contourpage.h \
    skeldocform.h \
    skelparamform.h \
    skelpage.h \
    roadsdocform.h \
    roadsparamform.h \
    roadspage.h \
    histoparamform.h \
    cartoparamform.h \
    histopage.h \
    histodocform.h \
    result.h \
    historesult.h \
    inthistoresult.h \
    inthistowidget.h \
    skelworker.h \
    doublehistoresult.h \
    doublehistowidget.h \
    rangeddoublehistowidget.h \
    doublerangedhistoresult.h \
    mapparamform.h \
    mapdocform.h \
    intmapresult.h \
    colorbadge.h \
    intcolorbadge.h \
    mapwidget.h \
    junctionmapwidget.h \
    mappage.h \
    intrangedmapresult.h \
    doublemapresult.h \
    roadmapwidget.h \
    mapresult.h \
    saveform.h \
    shapefil.h \
    reportpage.h \
    reportparamform.h \
    scalewidget.h \
    openingscalewidget.h \
    openglscalewidget.h

FORMS    += mainwindow.ui \
    statsform.ui \
    cropform.ui \
    regularizationform.ui \
    openingform.ui \
    contourform.ui \
    skelform.ui \
    roadsform.ui \
    introdocform.ui \
    introparamform.ui \
    histoparamform.ui \
    mapparamform.ui \
    cartoparamform.ui \
    histopage.ui \
    inthistowidget.ui \
    doublerangedform.ui \
    doublerangedhistoresult.ui \
    intmapresult.ui \
    saveform.ui \
    reportpage.ui \
    reportparamform.ui

RESOURCES += \
    images.qrc

