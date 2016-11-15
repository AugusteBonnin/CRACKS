#include "contourwidget.h"
#include "form.h"
#include "mainwindow.h"

#include <QApplication>
#include <QPainter>
#include <QRgb>
#include <QRgb>
#include <QSet>

ContourWidget::ContourWidget(Form *parent) : ScrollableOpenGLWidget(parent)
{
    mainWindow->quad_program = mainWindow->line_program = 0 ;
    mainWindow->contour_index_vbo  = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
}

ContourWidget::~ContourWidget()
{
    makeCurrent();
    mainWindow->quad_vbo.destroy();
    //mainWindow->contour_vbo.destroy();
    //mainWindow->contour_vao.destroy();
    //mainWindow->quad_vbo = mainWindow->contour_vbo = mainWindow->contour_vao = 0 ;
    delete mainWindow->texture;
    delete mainWindow->quad_program;
    delete mainWindow->line_program;
    doneCurrent();
}
/*
void ContourWidget::paintEvent(QPaintEvent *paintEvent)
{
    QPainter painter(this);
    painter.setTransform(imageToScreen) ;


    //contour
    QPen pen = painter.pen();
    pen.setWidth(2);
    pen.setColor(Qt::green);
    painter.setPen(pen);
    QVector<QPointF> &points = mainWindow->getContourVertices() ;
    QVector<unsigned int> &indices = mainWindow->getContourIndices() ;
    QVector<QLineF> lines ;
    for (int i = 0 ; i < indices.count() ; i+=2)
    {
        lines << QLineF(points[indices[i]],points[indices[i+1]]) ;
    }

    painter.drawLines(lines);


    glVertexPointer(3, GL_FLOAT, 0, mainWindow->getContourVertices()->constData());
    glDrawElements(GL_LINES,mainWindow->getContourIndices()->count(),GL_UNSIGNED_INT,mainWindow->getContourIndices()->constData());
    glDisableClientState(GL_VERTEX_ARRAY);

    //grid
    if ((mainWindow->getScale()>10.0/mainWindow->getOpenedImage()->height()))
    {
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_FLOAT, 0,grid_vertices.constData());
        glEnableClientState(GL_COLOR_ARRAY);
        glColorPointer(3, GL_UNSIGNED_BYTE, 0, grid_colors.constData());
        glDrawArrays(GL_LINES,0,grid_vertices.count());
        glDisableClientState(GL_COLOR_ARRAY);
        glDisableClientState(GL_VERTEX_ARRAY);

    }

}
*/
void ContourWidget::initializeGL()
{
    initializeOpenGLFunctions();


    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);


    //quad_program
    QOpenGLShader *vshader = new QOpenGLShader(QOpenGLShader::Vertex, this);
    char *vsrc =
            "attribute highp vec4 vertex;\n"
            "attribute mediump vec2 texCoord;\n"
            "varying mediump vec2 texc;\n"
            "uniform mediump mat4 matrix;\n"
            "void main(void)\n"
            "{\n"
            "    gl_Position = matrix * vertex;\n"
            "    texc = texCoord;\n"
            "}\n";
    vshader->compileSourceCode(vsrc);

    QOpenGLShader *fshader = new QOpenGLShader(QOpenGLShader::Fragment, this);
    char *fsrc =
            "uniform sampler2D texture;\n"
            "varying mediump vec2 texc;\n"
            "void main(void)\n"
            "{\n"
            "    gl_FragColor = texture2D(texture, texc);\n"
            "}\n";
    fshader->compileSourceCode(fsrc);

    mainWindow->quad_program = new QOpenGLShaderProgram(this);
    mainWindow->quad_program->addShader(vshader);
    mainWindow->quad_program->addShader(fshader);
    mainWindow->quad_program->bindAttributeLocation("vertex", PROGRAM_VERTEX_ATTRIBUTE);
    mainWindow->quad_program->bindAttributeLocation("texCoord", PROGRAM_TEXCOORD_ATTRIBUTE);
    mainWindow->quad_program->link();

    mainWindow->quad_program->bind();
    mainWindow->quad_program->setUniformValue("texture", 0);

    //line program
    vshader = new QOpenGLShader(QOpenGLShader::Vertex, this);
    vsrc =
            "attribute highp vec4 vertex;\n"
            "attribute lowp vec4 color;\n"
            "varying lowp vec4 interp_color;\n"
            "uniform mediump mat4 matrix;\n"
            "void main(void)\n"
            "{\n"
            "    gl_Position = matrix * vertex;\n"
            "    interp_color = color;\n"
            "}\n";
    vshader->compileSourceCode(vsrc);

    fshader = new QOpenGLShader(QOpenGLShader::Fragment, this);
    fsrc =
            "varying lowp vec4 interp_color;\n"
            "void main(void)\n"
            "{\n"
            "    gl_FragColor = interp_color;\n"
            "}\n";
    fshader->compileSourceCode(fsrc);

    mainWindow->line_program = new QOpenGLShaderProgram;
    mainWindow->line_program->addShader(vshader);
    mainWindow->line_program->addShader(fshader);
    mainWindow->line_program->bindAttributeLocation("vertex", PROGRAM_VERTEX_ATTRIBUTE);
    mainWindow->line_program->bindAttributeLocation("color", PROGRAM_COLOR_ATTRIBUTE);
    mainWindow->line_program->link();

    //mainWindow->line_program->bind();

    //Loads texture
    mainWindow->texture = new QOpenGLTexture(mainWindow->openedQImage);

    QVector<GLfloat> vertData;
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 2; ++j) {
            // vertex position
            vertData.append(i*(mainWindow->openedQImage.width())-.5f);
            vertData.append(j*(mainWindow->openedQImage.height())-.5f);
            // texture coordinate
            vertData.append(i);
            vertData.append(j);
        }
    }

    mainWindow->quad_vbo.create();
    mainWindow->quad_vbo.bind();
    mainWindow->quad_vbo.allocate(vertData.constData(), vertData.count() * sizeof(GLfloat));
}

void ContourWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);

    resetImage(mainWindow->openedQImage);

}

void ContourWidget::paintGL()
{
    glClearColor(.5f, .5f, .5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT );

    QMatrix4x4 m;
    m.scale(2*scale/width(),-2*scale/height());
    m.translate(-center.x(),-center.y());

    //background quad
    mainWindow->quad_program->bind();
    mainWindow->quad_vbo.bind();
    mainWindow->quad_program->setUniformValue("matrix", m);
    mainWindow->quad_program->enableAttributeArray(PROGRAM_VERTEX_ATTRIBUTE);
    mainWindow->quad_program->enableAttributeArray(PROGRAM_TEXCOORD_ATTRIBUTE);
    mainWindow->quad_program->setAttributeBuffer(PROGRAM_VERTEX_ATTRIBUTE, GL_FLOAT, 0, 2, 4 * sizeof(GLfloat));
    mainWindow->quad_program->setAttributeBuffer(PROGRAM_TEXCOORD_ATTRIBUTE, GL_FLOAT, 2 * sizeof(GLfloat), 2, 4 * sizeof(GLfloat));

    mainWindow->texture->bind();
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    mainWindow->quad_vbo.release();


    //contour lines
    if (mainWindow->getContourIndices().count())
    {
        mainWindow->line_program->bind();
        mainWindow->contour_vbo.bind();
        mainWindow->line_program->setUniformValue("matrix", m);
        mainWindow->line_program->enableAttributeArray(PROGRAM_VERTEX_ATTRIBUTE);
        mainWindow->line_program->enableAttributeArray(PROGRAM_COLOR_ATTRIBUTE);
        mainWindow->line_program->setAttributeBuffer(PROGRAM_VERTEX_ATTRIBUTE, GL_FLOAT, 0, 2, 6 * sizeof(GLfloat));
        mainWindow->line_program->setAttributeBuffer(PROGRAM_COLOR_ATTRIBUTE, GL_FLOAT, 2 * sizeof(GLfloat), 4, 6 * sizeof(GLfloat));

        mainWindow->contour_index_vbo->bind();
        glDrawElements(GL_LINES, mainWindow->getContourIndices().count(), GL_UNSIGNED_INT,0);
        mainWindow->contour_vbo.release();
        mainWindow->contour_index_vbo->release();
    }

}

void ContourWidget::updateThreshold(double treshold)
{
    bool NWPositive,NEPositive,SWPositive,SEPositive;

    int last_line_index[mainWindow->getOpenedImage()->width()-1];
    int last_left_index = -1;
    int last_column_index = -1 ;
    int first_column_index = -1 ;

    DoubleImage * doubleImage = mainWindow->getOpenedImage() ;

    QVector<QPointF> & contour_vertices = mainWindow->getContourVertices();
    contour_vertices.clear();
    QVector<unsigned int> & contour_indices = mainWindow->getContourIndices();
    contour_indices.clear();
    QVector<unsigned int> & next_point_index = mainWindow->getNextPointIndex() ;
    next_point_index.clear();
    QVector<unsigned int> & prev_point_index = mainWindow->getPrevPointIndex() ;
    prev_point_index.clear() ;

    NWPositive = (doubleImage->pixel(0,0)>treshold) ;
    if (NWPositive)
    {
        QPointF corner(0,0);
        contour_vertices.append(corner);
        next_point_index.append(-1);
        first_column_index = 0 ;
    }
    //first line
    for (int i = 1 ; i < doubleImage->width() ; i ++)
    {
        NEPositive = (doubleImage->pixel(i,0)>treshold) ;

        if (NEPositive)
        {
            if (NWPositive)
            {
                //link last end to new end
                next_point_index[next_point_index.count()-1] = contour_vertices.count() ;
                QPointF end(i,0);
                contour_vertices.append(end);
                next_point_index.append(-1);
            }
            else
            {
                //link interp to positive right end
                QPointF p = point_lerp(i-1,0,doubleImage->pixel(i-1,0),i,0,doubleImage->pixel(i,0),treshold);
                contour_vertices.append(p);
                next_point_index.append(contour_vertices.count());
                QPointF end(i,0);
                contour_vertices.append(end);
                next_point_index.append(-1);
                last_line_index[i] = contour_vertices.count() - 2 ;
            }

            last_column_index = contour_vertices.count() - 1 ;
        }
        else
        {
            if (NWPositive)
            {
                //link last corner to inter point
                next_point_index[next_point_index.count()-1] = contour_vertices.count() ;
                //link interp point to nothing
                QPointF p = point_lerp(i-1,0,doubleImage->pixel(i-1,0),i,0,doubleImage->pixel(i,0),treshold);
                contour_vertices.append(p);
                next_point_index.append(-1);
                last_line_index[i] = contour_vertices.count() - 1 ;

            }
            else
            {
                //do Nothing
            }

        }

        NWPositive = NEPositive ;
    }

    //other lines
    mainWindow->progress->setValue(0);
    mainWindow->progress->setMaximum(doubleImage->height()-1);
    for (int j = 1 ; j < doubleImage->height() ; j++)
    {
        mainWindow->progress->setValue(j);
        qApp->processEvents() ;
        //first W Edge
        NWPositive = (doubleImage->pixel(0,j-1)>treshold) ;
        SWPositive = (doubleImage->pixel(0,j)>treshold) ;
        if (SWPositive)
        {
            if (NWPositive)
            {
                //link SW to NW
                contour_vertices.append(QPointF(0,j));
                next_point_index.append(first_column_index);
                first_column_index = contour_vertices.count() - 1 ;
            }
            else
            {
                //link SW to interp
                contour_vertices.append(QPointF(0,j));
                next_point_index.append(contour_vertices.count());
                QPointF p = point_lerp(0,j-1,doubleImage->pixel(0,j-1),0,j,doubleImage->pixel(0,j),treshold);
                contour_vertices.append(p);
                next_point_index.append(-1);
                first_column_index = contour_vertices.count() - 2 ;
                last_left_index = contour_vertices.count() - 1 ;

            }
        }
        else
        {
            if (NWPositive)
            {
                //link interp to NW
                QPointF p = point_lerp(0,j-1,doubleImage->pixel(0,j-1),0,j,doubleImage->pixel(0,j),treshold);
                contour_vertices.append(p);
                next_point_index.append(first_column_index);
                last_left_index = contour_vertices.count() - 1 ;
            }
            else
            {
                //Do Nothing

            }
        }

        // columns
        for (int i = 1 ; i < doubleImage->width() ; i++)
        {
            SEPositive = (doubleImage->pixel(i,j)>treshold) ;
            NEPositive = (doubleImage->pixel(i,j-1)>treshold) ;
            if (SEPositive)
            {
                if (NEPositive)
                {
                    //SE+ NE+
                    if (NWPositive)
                    {
                        if(SWPositive)
                        {//Do nothing
                        }
                        else
                        {
                            // SW-
                            //link S->W
                            QPointF S = point_lerp(i-1,j,doubleImage->pixel(i-1,j),i,j,doubleImage->pixel(i,j),treshold);
                            next_point_index.append(last_left_index);
                            contour_vertices.append(S);
                            last_line_index[i] = contour_vertices.count() - 1 ;
                        }
                    }
                    else
                    {
                        if(SWPositive)
                        {//SE+ NE+ NW- SW +
                            //link W->N
                            next_point_index[last_left_index] = last_line_index[i] ;
                        }
                        else
                        {
                            //SE+ NE+ NW- SW -
                            //link S->N
                            QPointF S = point_lerp(i-1,j,doubleImage->pixel(i-1,j),i,j,doubleImage->pixel(i,j),treshold);
                            contour_vertices.append(S);
                            next_point_index.append(last_line_index[i]);

                            last_line_index[i] = contour_vertices.count() - 1 ;
                        }
                    }
                }
                else
                {
                    //SE+ , NE-
                    if (NWPositive)
                    {
                        if(SWPositive)
                        {
                            //SE+ , NE- , SW+ , NW +
                            //link N->E
                            QPointF E = point_lerp(i,j-1,doubleImage->pixel(i,j-1),i,j,doubleImage->pixel(i,j),treshold);
                            contour_vertices.append(E);
                            next_point_index.append(-1);
                            next_point_index[last_line_index[i]] = contour_vertices.count()-1 ;
                            last_left_index = contour_vertices.count()-1 ;
                        }
                        else
                        {
                            //SE+ , NE- , SW- , NW +
                            /*
                                     * //link N->W
                                    next_point_index[last_line_index[i]] = last_left_index ;
                                    //link S->E
                                    QPointF E = point_lerp(i,j-1,doubleImage->pixel(i,j-1),i,j,doubleImage->pixel(i,j),treshold);
                                    contour_vertices.append(E);
                                    next_point_index.append(-1);

                                    QPointF S = point_lerp(i-1,j,doubleImage->pixel(i-1,j),i,j,doubleImage->pixel(i,j),treshold);
                                    next_point_index.append(contour_vertices.count()-1);
                                    contour_vertices.append(S);

                                    last_line_index[i] = contour_vertices.count()-1 ;
                                    last_left_index = contour_vertices.count()-2 ;
                                    */
                            //link N->E
                            //link S->W
                            QPointF E = point_lerp(i,j-1,doubleImage->pixel(i,j-1),i,j,doubleImage->pixel(i,j),treshold);
                            contour_vertices.append(E);
                            next_point_index.append(-1);

                            QPointF S = point_lerp(i-1,j,doubleImage->pixel(i-1,j),i,j,doubleImage->pixel(i,j),treshold);
                            next_point_index.append(last_left_index);
                            contour_vertices.append(S);

                            next_point_index[last_line_index[i]] = contour_vertices.count() - 2;
                            last_line_index[i] = contour_vertices.count()-1 ;
                            last_left_index = contour_vertices.count()-2 ;

                        }
                    }
                    else
                    {
                        if(SWPositive)
                        {
                            //SE+ , NE- , SW+ , NW -
                            //link W->E
                            next_point_index[last_left_index] = contour_vertices.count() ;
                            QPointF E = point_lerp(i,j-1,doubleImage->pixel(i,j-1),i,j,doubleImage->pixel(i,j),treshold);
                            contour_vertices.append(E);
                            next_point_index.append(-1);
                            last_left_index = contour_vertices.count()-1 ;
                        }
                        else
                        {
                            //SE+ , NE- , SW- , NW -
                            //link S->E
                            QPointF E = point_lerp(i,j-1,doubleImage->pixel(i,j-1),i,j,doubleImage->pixel(i,j),treshold);
                            next_point_index.append(-1);
                            contour_vertices.append(E);

                            QPointF S = point_lerp(i-1,j,doubleImage->pixel(i-1,j),i,j,doubleImage->pixel(i,j),treshold);
                            next_point_index.append(contour_vertices.count()-1);
                            contour_vertices.append(S);

                            last_line_index[i] = contour_vertices.count()-1 ;
                            last_left_index = contour_vertices.count()-2 ;
                        }
                    }

                }
            }
            else
            {
                if (NEPositive)
                {
                    // NOT SEPositive , NEPositive

                    if (NWPositive)
                    {
                        if(SWPositive)
                        {
                            //link E->S
                            QPointF E = point_lerp(i,j-1,doubleImage->pixel(i,j-1),i,j,doubleImage->pixel(i,j),treshold);
                            contour_vertices.append(E);
                            next_point_index.append(contour_vertices.count());
                            QPointF S = point_lerp(i-1,j,doubleImage->pixel(i-1,j),i,j,doubleImage->pixel(i,j),treshold);
                            contour_vertices.append(S);
                            next_point_index.append(-1);
                            last_line_index[i] = contour_vertices.count()-1 ;
                            last_left_index = contour_vertices.count()-2 ;

                        }
                        else
                        {
                            //link E->W
                            QPointF E = point_lerp(i,j-1,doubleImage->pixel(i,j-1),i,j,doubleImage->pixel(i,j),treshold);
                            contour_vertices.append(E);
                            next_point_index.append(last_left_index);
                            last_left_index = contour_vertices.count()-1 ;
                        }
                    }
                    else
                    {
                        //Not SE , NOt NW , NE
                        if(SWPositive)
                        {
                            //link W->N
                            next_point_index[last_left_index] = last_line_index[i] ;
                            //link E->S
                            QPointF E = point_lerp(i,j-1,doubleImage->pixel(i,j-1),i,j,doubleImage->pixel(i,j),treshold);
                            contour_vertices.append(E);
                            next_point_index.append(contour_vertices.count());
                            QPointF S = point_lerp(i-1,j,doubleImage->pixel(i-1,j),i,j,doubleImage->pixel(i,j),treshold);
                            contour_vertices.append(S);
                            next_point_index.append(-1);

                            last_line_index[i] = contour_vertices.count()-1 ;
                            last_left_index = contour_vertices.count()-2 ;
                        }
                        else
                        {
                            //Not SE , NOt NW , NE , not SW

                            //link E->N
                            QPointF E = point_lerp(i,j-1,doubleImage->pixel(i,j-1),i,j,doubleImage->pixel(i,j),treshold);
                            contour_vertices.append(E);
                            next_point_index.append(last_line_index[i]);
                            last_left_index = contour_vertices.count()-1 ;
                        }
                    }
                }
                else
                {
                    //SE- NE-
                    if (NWPositive)
                    {
                        if(SWPositive)
                        {
                            //SE- NE- SW+ NW+
                            //link N->S
                            QPointF S = point_lerp(i-1,j,doubleImage->pixel(i-1,j),i,j,doubleImage->pixel(i,j),treshold);
                            next_point_index[last_line_index[i]] = contour_vertices.count() ;
                            contour_vertices.append(S);
                            next_point_index.append(-1);
                            last_line_index[i] = contour_vertices.count()-1 ;

                        }
                        else
                        {
                            //SE- NE- SW- NW+
                            //link N->W
                            next_point_index[last_line_index[i]] = last_left_index ;
                        }
                    }
                    else
                    {
                        // SE- , NW- , NE-
                        if(SWPositive)
                        {
                            // SE- , NW- , NE- , SW+
                            //link W->S
                            next_point_index[last_left_index] = contour_vertices.count() ;
                            QPointF S = point_lerp(i-1,j,doubleImage->pixel(i-1,j),i,j,doubleImage->pixel(i,j),treshold);
                            contour_vertices.append(S);
                            next_point_index.append(-1);

                            last_line_index[i] = contour_vertices.count() - 1 ;
                        }
                        else
                        {
                            // SE- , NW- , NE- , SW-
                            //Do nothing
                        }
                    }
                }

            }
            NWPositive = NEPositive ;
            SWPositive = SEPositive ;
        }
        //last E edge
        if (SEPositive)
        {
            if (NEPositive)
            {

                next_point_index[last_column_index] = next_point_index.count() ;
                QPointF p(doubleImage->width()-1,j);
                contour_vertices.append(p);
                next_point_index.append(-1);
                last_column_index = contour_vertices.count()-1 ;
            }
            else
            {
                next_point_index[last_left_index] = next_point_index.count() ;
                QPointF corner(doubleImage->width()-1,j);
                contour_vertices.append(corner);
                next_point_index.append(-1) ;
                last_column_index = contour_vertices.count()-1 ;

            }
        }
        else
        {
            if (NEPositive)
            {
                next_point_index[last_column_index] = last_left_index ;
            }

        }
    }

    //SW corner
    SWPositive = (doubleImage->pixel(0,doubleImage->height()-1)>treshold) ;
    if (SWPositive)
    {
        QPointF corner(0,doubleImage->height()-1);
        contour_vertices.append(corner);
        next_point_index.append(first_column_index);
        last_left_index = contour_vertices.count() - 1 ;
    }
    //last line
    for (int i = 1 ; i < doubleImage->width() ; i ++)
    {
        SEPositive = (doubleImage->pixel(i,doubleImage->height()-1)>treshold) ;

        if (SEPositive)
        {
            if (SWPositive)
            {
                //link new end to last end
                QPointF end(i,doubleImage->height()-1);
                contour_vertices.append(end);
                next_point_index.append(contour_vertices.count()-2);
            }
            else
            {
                //link positive right end to interp
                QPointF end(i,doubleImage->height()-1);
                contour_vertices.append(end);
                next_point_index.append(last_line_index[i]);

            }
            last_left_index = contour_vertices.count() - 1 ;
        }
        else
        {
            if (SWPositive)
            {
                //link inter point to last corner
                next_point_index[last_line_index[i]] = last_left_index ;
            }
            else
            {
                //do Nothing
            }

        }

        SWPositive = SEPositive ;
    }

    //SE corner
    if (SEPositive)
    {
        if (SWPositive)
        {
            //link last column to last_end
            next_point_index[last_column_index] = last_left_index ;

        }
        else
        {
            //link positive right end to interp
            next_point_index[last_column_index] = last_line_index[doubleImage->width()-1] ;

        }
    }

    prev_point_index.resize(next_point_index.count());
    for (int i = 0 ; i  < next_point_index.count() ; i++)
    {
        prev_point_index[next_point_index[i]] = i ;
    }

    QVector<bool> not_counted(contour_vertices.count(),true);

    for (int i = 0 ; i  < next_point_index.count() ; i++)
    {
        int i0 = prev_point_index[i] ;
        int i2 = next_point_index[i] ;

        QPointF & p0 = contour_vertices[i0] ;
        QPointF & p1 = contour_vertices[i] ;
        QPointF & p2 = contour_vertices[i2] ;

        if (isFramePoint(doubleImage,p0)&&isFramePoint(doubleImage,p1)&&
                isFramePoint(doubleImage,p2)&&(!isCornerPoint(doubleImage,p1)))
        {
            prev_point_index[i2] = i0;
            next_point_index[i0] = i2;
            not_counted[i] = false ;
        }
    }

    //count connected components
    QVector < unsigned int> & connected_components_start = mainWindow->getConnectedComponentsStarts();
    connected_components_start.clear();

    mainWindow->progress->setValue(0) ;
    mainWindow->progress->setMaximum(contour_vertices.count()) ;

    contour_indices.clear() ;

    int current = 0 ;
    while (current!=-1)
    {
        not_counted[current] = false ;

        unsigned int count = 1 ;
        QVector<int> history ;

        int found = -1 ;

        do {
            history << current ;
            contour_indices.append(current);
            contour_indices.append(next_point_index[current]);
            not_counted[current] = false ;

            current = next_point_index[current];
            count++ ;
            found = history.indexOf(current) ;
        } while (found==-1);

        current = not_counted.indexOf(true);

        connected_components_start.append(history[found]);

        mainWindow->progress->setValue(mainWindow->progress->value()+count);
        qApp->processEvents();
    }


    //make contour vertices vbo
    makeCurrent() ;

    if (mainWindow->contour_vbo.isCreated())
        mainWindow->contour_vbo.destroy() ;

    mainWindow->contour_vbo.create() ;
    mainWindow->contour_vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
    QVector<float> vertices ;
    for (int i = 0 ; i < contour_vertices.count() ; i++)
    {
        vertices << contour_vertices[i].x() << contour_vertices[i].y () << 0.0f << 1.0f << 0.0f << 1.0f ;
    }
    mainWindow->contour_vbo.bind();
    mainWindow->contour_vbo.allocate(vertices.constData(), vertices.count() * sizeof(GLfloat));
    vertices.clear() ;

    //make contour index vbo
    if (mainWindow->contour_index_vbo->isCreated())
        mainWindow->contour_index_vbo->destroy() ;

    mainWindow->contour_index_vbo->create() ;
    mainWindow->contour_index_vbo->setUsagePattern(QOpenGLBuffer::StaticDraw);
    QVector<unsigned int> indices ;
    for (int i = 0 ; i < contour_indices.count() ; i+=2)
    {
        indices << contour_indices[i] << contour_indices[i+1] ;
    }
    mainWindow->contour_index_vbo->bind();
    mainWindow->contour_index_vbo->allocate(indices.constData(), indices.count() * sizeof(unsigned int));
    indices.clear() ;

    //make grid
    QImage grid_image(mainWindow->openedQImage);
    int p = 0 ;
    for (int j = 0 ; j < doubleImage->height() ; j++)
        for (int i = 0 ; i < doubleImage->width() ; i++)
            if (doubleImage->pixel(i,j)>treshold)
            {

                 grid_image.setPixel(i,j,0xFFFF0000);
            }
            else
            {
                QRgb pixel = 0xFF0000FF ;
                grid_image.setPixel(i,j,pixel);
            }
    if (mainWindow->texture)
        delete mainWindow->texture ;
    mainWindow->texture = new QOpenGLTexture(grid_image);
     mainWindow->texture->setBorderColor(127,127,127,255);
     mainWindow->texture->setWrapMode(QOpenGLTexture::DirectionR,QOpenGLTexture::ClampToEdge);
     mainWindow->texture->setWrapMode(QOpenGLTexture::DirectionS,QOpenGLTexture::ClampToEdge);
    doneCurrent();



    /*
    //simplify holes
    simplified_holes.clear() ;
    qreal mean_sides = 0 ;
    QHash<int,int> histo;
    for (int i = 0 ; i < connected_components_start.count() ; i++)
    {
        QPolygonF poly;
        int current = connected_components_start[i] ;
        do {
            poly << contour_vertices[current] ;
            current = next_point_index[current] ;
        } while (current!=connected_components_start[i]) ;
        QPointF center = poly.boundingRect().center() ;
        QLineF AB(center,poly[0]) ;
        QLineF BC(poly[0],poly[1]) ;
        qreal angle = AB.angleTo(BC);
        if ((sinf(angle*M_PI/180)<0))
            continue ;

        bool something_happend ;
        do {
            something_happend = false ;

            int j = 0 ;
            float min = INFINITY ;
            int min_index = -1 ;
            while (j<poly.count())
            {
                QLineF AB(poly[j],poly[(j+1)%poly.count()]) ;
                QLineF AC(poly[j],poly[(j+2)%poly.count()]) ;
                float area = sinf(AB.angleTo(AC)*M_PI/180)*AC.length() ;
                if (area<min)
                {
                    min = area ;
                    min_index = (j+1)%poly.count() ;
                }
                j++ ;

            }
            if (min < 4 )
            {
                poly.remove(min_index);
                something_happend = true ;
            }
        } while(something_happend) ;

        mean_sides += poly.count() ;

        histo.insert(poly.count(),histo.value(poly.count(),0)+1) ;

        simplified_holes << poly ;
    }

    mean_sides /= simplified_holes.count() ;

    qDebug() << "found " << simplified_holes.count() << " well defined holes with " << mean_sides << " sides " ;
    QList<int> keys = histo.keys() ;
for (int i = 0 ; i < keys.count() ; i++)
    qDebug() << "found " << histo[keys[i]] << " holes with " << keys[i] << " sides " << (histo[keys[i]]*100.0/simplified_holes.count()) << "%" ;
*/
    repaint() ;
}
