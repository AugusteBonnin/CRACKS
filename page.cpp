#include "page.h"

#include <QPainter>

Page::Page(MainWindow * parent) : Form(parent) , computed(false)
{

}

void Page::screenshot(QImage &image)
{
    image = QImage(width(),height(),QImage::Format_ARGB32);

    QPainter painter(&image);
    render(&painter);
}

void Page::setDocForm(DocForm *value)
{
    docForm = value;
}

void Page::setParamForm(ParamForm *value)
{
    paramForm = value;
}

