#-------------------------------------------------
#
# Project created by QtCreator 2016-03-31T15:32:10
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.12

TARGET = CRACKS!

TEMPLATE = app

LIBS += -L/usr/local/lib  -lCGAL  -lCGAL_Core -lgmp -lmpfr
LIBS +=  -lboost_thread -lboost_system -lz
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
    introparamform.cpp \
    docform.cpp \
    paramform.cpp \
    page.cpp \
    intropage.cpp \
    cropparamform.cpp \
    croppage.cpp \
    regularizationparamform.cpp \
    regularizationpage.cpp \
    openingparamform.cpp \
    openingpage.cpp \
    contourparamform.cpp \
    contourpage.cpp \
    skelparamform.cpp \
    skelpage.cpp \
    roadsparamform.cpp \
    roadspage.cpp \
    histoparamform.cpp \
    histopage.cpp \
    result.cpp \
    historesult.cpp \
    inthistoresult.cpp \
    inthistowidget.cpp \
    doublehistoresult.cpp \
    doublehistowidget.cpp \
    rangeddoublehistowidget.cpp \
    doublerangedhistoresult.cpp \
    mapparamform.cpp \
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
    openglscalewidget.cpp \
    histopagefactories.cpp \
    mappagefactory.cpp \
    frame.cpp \
    skelworker.cpp \
    skelworker2.cpp \
    constraineddelaunaytriangulation.cpp \
    logloghistoresult.cpp \
    logloghistowidget.cpp

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
    introparamform.h \
    docform.h \
    paramform.h \
    page.h \
    intropage.h \
    cropparamform.h \
    croppage.h \
    regularizationparamform.h \
    regularizationpage.h \
    openingparamform.h \
    openingpage.h \
    contourparamform.h \
    contourpage.h \
    skelparamform.h \
    skelpage.h \
    roadsparamform.h \
    roadspage.h \
    histoparamform.h \
    histopage.h \
    result.h \
    historesult.h \
    inthistoresult.h \
    inthistowidget.h \
    doublehistoresult.h \
    doublehistowidget.h \
    rangeddoublehistowidget.h \
    doublerangedhistoresult.h \
    mapparamform.h \
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
    openglscalewidget.h \
    histopagefactories.h \
    mappagefactory.h \
    frame.h \
    skelworker.h \
    skelworker2.h \
    constraineddelaunaytriangulation.h \
    logloghistoresult.h \
    logloghistowidget.h

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
    histopage.ui \
    inthistowidget.ui \
    doublerangedform.ui \
    doublerangedhistoresult.ui \
    intmapresult.ui \
    saveform.ui \
    reportpage.ui \
    reportparamform.ui

RESOURCES += \
    resources.qrc

DISTFILES += \
    introduction.html \
    bilan.html \
    cartes.html \
    contour.html \
    histogrammes.html \
    ouverture.html \
    recadrage.html \
    regularisation.html \
    squelettisation.html \
    voies.html \
    aide.html

