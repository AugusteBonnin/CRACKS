#include "roadswidget.h"
#include "mainwindow.h"
#include "inthistogramlabel.h"
#include "histogramlabel.h"
#include <QApplication>
#include <QDebug>
#include <QPainter>
#include <cstdlib>
#include <limits.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/ch_graham_andrew.h>
#include <CGAL/convex_hull_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 Point_2;

RoadsWidget::RoadsWidget(Page *parent) : ScrollableOpenGLWidget(parent) ,
    junctions(mainWindow->getJunctions()) ,
    double_sided_edges(mainWindow->getDoubleSidedEdges()) ,
    roads_junctions(mainWindow->getRoadsJunctions()) ,
    roads_edges(mainWindow->getRoadsEdges()) ,
    index_junction(mainWindow->getIndexJunction()) ,
    skel_children(mainWindow->getSkelChildren()) ,
    skel_vertices(mainWindow->getSkelVertices()) ,
    skel_distance(mainWindow->getSkelDistanceToBoundary()) ,
    pre_roads_index_vbo(mainWindow->pre_roads_index_vbo) ,
    pre_roads_vbo(mainWindow->pre_roads_vbo) ,
    pre_roads_index_vbo_start(mainWindow->pre_roads_index_vbo_start) ,
    pre_roads_index_vbo_end(mainWindow->pre_roads_index_vbo_end),
    pre_junctions_vbos(mainWindow->pre_junctions_vbos)
{

}

void RoadsWidget::initializeGL()
{
    initializeOpenGLFunctions();

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    glLineWidth(3);

    mainWindow->roads_index_vbo = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer) ;
}

void RoadsWidget::resizeGL(int w, int h)
{
    glViewport(0,0,w,h) ;
}

void RoadsWidget::paintGL()
{
    glClearColor(.5f,.5f,.5f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

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

    //junctions contours
    for (int i = 0 ; i < mainWindow->junctions_contours_vbos.count() ; i++)
    {
        mainWindow->junctions_contours_vbos[i]->bind();
        mainWindow->line_program->setUniformValue("matrix", m);
        mainWindow->line_program->enableAttributeArray(PROGRAM_VERTEX_ATTRIBUTE);
        mainWindow->line_program->enableAttributeArray(PROGRAM_COLOR_ATTRIBUTE);
        mainWindow->line_program->setAttributeBuffer(PROGRAM_VERTEX_ATTRIBUTE, GL_FLOAT, 0, 2, 6 * sizeof(GLfloat));
        mainWindow->line_program->setAttributeBuffer(PROGRAM_COLOR_ATTRIBUTE, GL_FLOAT, 2 * sizeof(GLfloat), 4, 6 * sizeof(GLfloat));
        glDrawArrays(GL_LINE_STRIP,0,mainWindow->junctions_contours_vbos[i]->size()/(6*sizeof(float)));
        mainWindow->junctions_contours_vbos[i]->release();
    }
    //junctions colors
    for (int i = 0 ; i < mainWindow->junctions_bg_vbos.count() ; i++)
    {
        mainWindow->junctions_bg_vbos[i]->bind();
        mainWindow->line_program->setUniformValue("matrix", m);
        mainWindow->line_program->enableAttributeArray(PROGRAM_VERTEX_ATTRIBUTE);
        mainWindow->line_program->enableAttributeArray(PROGRAM_COLOR_ATTRIBUTE);
        mainWindow->line_program->setAttributeBuffer(PROGRAM_VERTEX_ATTRIBUTE, GL_FLOAT, 0, 2, 6 * sizeof(GLfloat));
        mainWindow->line_program->setAttributeBuffer(PROGRAM_COLOR_ATTRIBUTE, GL_FLOAT, 2 * sizeof(GLfloat), 4, 6 * sizeof(GLfloat));
        glDrawArrays(GL_POLYGON,0,mainWindow->junctions_bg_vbos[i]->size()/(6*sizeof(float)));
        mainWindow->junctions_bg_vbos[i]->release();
    }
    //roads
    if (mainWindow->roads_vbo.isCreated())
    {
        mainWindow->line_program->bind();
        mainWindow->roads_vbo.bind();
        mainWindow->line_program->setUniformValue("matrix", m);
        mainWindow->line_program->enableAttributeArray(PROGRAM_VERTEX_ATTRIBUTE);
        mainWindow->line_program->enableAttributeArray(PROGRAM_COLOR_ATTRIBUTE);
        mainWindow->line_program->setAttributeBuffer(PROGRAM_VERTEX_ATTRIBUTE, GL_FLOAT, 0, 2, 6 * sizeof(GLfloat));
        mainWindow->line_program->setAttributeBuffer(PROGRAM_COLOR_ATTRIBUTE, GL_FLOAT, 2 * sizeof(GLfloat), 4, 6 * sizeof(GLfloat));

        mainWindow->roads_index_vbo->bind();
        glDrawElements(GL_LINES, mainWindow->roads_index_vbo->size()/sizeof(unsigned int), GL_UNSIGNED_INT,0);
        mainWindow->roads_vbo.release();
        mainWindow->roads_index_vbo->release();
    }

    /*
    QTransform imageToScreen(scale,0,0,scale,
                             (width()>>1)-center.x()*scale,(height()>>1)-center.y()*scale);

    QPainter painter(this) ;
    painter.setPen(Qt::black);
    QFont font=painter.font() ;
    font.setPointSize(18);
    painter.setFont(font);

    for (int i = 0 ; i < cycles_centers.count() ; i++)
    {
        QPointF p = imageToScreen.map(cycles_centers[i]) ;
        painter.drawText(p,QString("%1").arg(cycles_lengths[i]));
        QPolygonF poly = imageToScreen.map(cycles_contours[i]) ;
        painter.drawPolygon(poly);
    }

    painter.end() ;
    */
}


void RoadsWidget::buildRoads(double radiusFactor,double threshold_on_B)
{
    this->radiusFactor = radiusFactor ;

    junctions.clear();
    double_sided_edges.clear();
    roads_junctions.clear();
    roads_edges.clear();
    index_junction.clear();

    mainWindow->histoIntData.clear();
    mainWindow->histoDoubleData.clear();

    //line strings computing
    mainWindow->progress->setMaximum(skel_children.count());

    int total_vertices = 0 ;

    for (int i = 0 ; i  < skel_children.count() ; i++)
    {
        if((skel_children[i].count()==1)||(skel_children[i].count()>=3))
        {
            explorePoint(i);
            total_vertices++ ;
        }
        mainWindow->progress->setValue(i);
        qApp->processEvents();
    }

    int count =  0 ;
    for (int i = 0 ; i < junctions.count() ; i++)
    {
        if (junctions[i].centers_indices.count())
        {
            count ++ ;
            junctions[i].mean_radius = 0 ;

            for (int j = 0 ; j < junctions[i].centers_indices.count() ; j++)
                junctions[i].mean_radius += skel_distance[junctions[i].centers_indices[j]] ;

            junctions[i].mean_radius /= junctions[i].centers_indices.count() ;
        }
    }

    //cout << total_vertices << " total_vertices " << endl;
    //cout << double_sided_edges.count() << " double_sided_edges" << endl;

    for (int i = 0 ; i < double_sided_edges.count() ; i++)
    {
        DoubleSidedEdge &edge = double_sided_edges[i] ;
        edge.first_junction = index_junction.value(edge.first_index);
        edge.second_junction = index_junction.value(edge.second_index);

    }

    //    for (int i = 0 ; i < junctions.count() ; i++)
    //    {
    //        bool something_happend = true ;
    //        while (something_happend)
    //        {
    //            something_happend = false ;
    //            for (int j = 0 ; j < junctions[i].arrivals.count() ; j++)
    //            {
    //                DoubleSidedEdge edge = double_sided_edges[junctions[i].arrivals[j].edge] ;
    //                if (edge.first_junction==edge.second_junction)
    //                {
    //                    something_happend=true;
    //                    junctions[i].arrivals.remove(j);
    //                    //cout << "null edge contraction\n" ;
    //                    break ;
    //                }
    //            }
    //        }
    //    }

    //cout << count << " distinct junctions on "<< junctions.count() << endl;

    const float delta_width_constant = logf(2);
    //HistogramLabel * histo = new HistogramLabel(640,480,180,2,5,5);
    QVector<double> data;
    for (int i = 0 ; i < junctions.count() ; i++)
    {
        QVector<EdgePair> edge_pairs;
        const QVector<Arrival> & arrivals = junctions[i].arrivals ;
        for (int j = 0 ; j < arrivals.count() - 1; j++)

            for (int k = j+1 ; k < arrivals.count() ; k++)
            {
                if (isnan(arrivals[j].angle)||isnan( arrivals[k].angle)) qDebug() << "NAN in edge pairs" ;
                else
                {
                    float a = arrivals[j].angle-arrivals[k].angle;
                    while (a > 180) a-=360 ;
                    while (a < -180) a+=360 ;
                    a = fabs(a) ;
                    data << a ;
                }

                float A = arrivals[j].angle - arrivals[k].angle - 180 ;
                while (A>180)
                    A -= 360 ;
                while(A<-180)
                    A += 360 ;
                A = fabs(A) ;
                A /= 150 ;

                float first_width = (double_sided_edges[arrivals[j].edge].first_junction==i)?
                            double_sided_edges[arrivals[j].edge].first_width : double_sided_edges[arrivals[j].edge].second_width ;
                float second_width = (double_sided_edges[arrivals[k].edge].first_junction==i)?
                            double_sided_edges[arrivals[k].edge].second_width : double_sided_edges[arrivals[k].edge].first_width ;
                float C = fabs(logf(first_width/second_width)/delta_width_constant);

                if ((A<1.0f)&&(C<1.0f))
                {
                    EdgePair element;
                    element.i = arrivals[j].edge ;
                    element.j = arrivals[k].edge ;
                    element.point_i = arrivals[j].point ;
                    element.point_j = arrivals[k].point ;
                    QLineF AB(arrivals[j].point,arrivals[k].point);
                    float phi1 = arrivals[j].angle - AB.angle() -180;
                    while (phi1<=-180) phi1 += 360 ;
                    while (phi1>180) phi1 -= 360 ;
                    float phi2 = arrivals[k].angle - AB.angle() ;
                    while (phi2<=-180) phi2 += 360 ;
                    while (phi2>180) phi2 -= 360 ;

                    //element.B = AB.length()*(fabs(sinf(M_PI*phi1/180)/first_width))+fabs(sinf(M_PI*phi2/180)/second_width) ;
                    element.B = fabs(phi1)+fabs(phi2) ;
                    //element.B = fabs(sinf(M_PI*phi1/180)+fabs(sinf(M_PI*phi2/180)) ;
                    edge_pairs.append(element);

                    //histo->insertValue(element.B);
                }
            }


        //        for (int j = 0 ; j < arrivals.count() - 1; j++)
        //            cout << "Junction " << i << " edge " << arrivals[j].edge << " angle " << arrivals[j].angle << endl ;
        QVector<QLineF> tree;
        if (edge_pairs.count())
        {
            qSort(edge_pairs);
            //cout << "assertion " << edge_pairs[0].B << " < " << edge_pairs[edge_pairs.count()-1].B << endl ;
            QSet<int> already_used_edge ;
            for (int j = 0 ; j < edge_pairs.count() ; j++)
            {
                if ((!already_used_edge.contains(edge_pairs[j].i))&&
                        (!already_used_edge.contains(edge_pairs[j].j))&&
                        (edge_pairs[j].B<threshold_on_B))
                {
                    tree << QLineF(edge_pairs[j].point_i, edge_pairs[j].point_j);

                    already_used_edge.insert(edge_pairs[j].i);
                    already_used_edge.insert(edge_pairs[j].j);
                    junctions[i].equivalent.insert(edge_pairs[j].i , edge_pairs[j].j ) ;
                    junctions[i].equivalent.insert(edge_pairs[j].j , edge_pairs[j].i ) ;

                    //                    cout << "Junction " << i << " : pair found with distance "<< edge_pairs[j].B << "bbetwwen " << edge_pairs[j].i <<
                    //                            " and" << edge_pairs[j].j << endl ;
                }
                /*else
                    if ((edge_pairs[j].distance<60))
                        cout << "Junction " << i << " : edge already used : "<<
                             edge_pairs[j].i <<
                                " and " << edge_pairs[j].j << " with distance "<< edge_pairs[j].distance << endl ;
                else
                        cout << "Junction " << i << " : pair too distant with distance "<< edge_pairs[j].distance << " bbetwwen " << edge_pairs[j].i <<
                                " and " << edge_pairs[j].j << endl ;
                                */
            }
            if (tree.count())
            {
                for (int j = 0 ; j < arrivals.count() ; j++)
                    if (!junctions[i].equivalent.contains(arrivals[j].edge))
                    {
                        if (isnan(arrivals[j].angle)) qDebug() << "NAN in inter" ;
                        float radius = 5*junctions[i].mean_radius ;
                        //                        qDebug() << "radius" << radius ;
                        QLineF l=QLineF::fromPolar(radius,arrivals[j].angle+180);
                        l.translate(arrivals[j].point);

                        qreal min_d = INFINITY ;
                        QPointF min_d_point;
                        QPointF current_point ;
                        for (int k = 0 ; k < tree.count() ; k++)
                        {
                            int result = l.intersect(tree[k],&current_point) ;
                            if (result==0) qDebug() << " 0 " << arrivals[j].angle << tree[k].angle();
                            if ((result==QLineF::BoundedIntersection))
                            {
                                qreal d = QLineF(current_point,l.p1()).length();
                                if (d<min_d)
                                {
                                    min_d_point = current_point;
                                    min_d = d ;
                                }
                            }
                        }

                        if (min_d<INFINITY)
                        {
                            skel_vertices << min_d_point ;
                            QVector<int> & str = double_sided_edges[arrivals[j].edge].truncated_str ;
                            if (!arrivals[j].str_inverted)
                                str.push_front(skel_vertices.count() -1);
                            else
                                str.push_back(skel_vertices.count() -1);

                            QLineF r(l.p1(),min_d_point);
                            tree << r ;
                        }

                    }
            }
            else
            {
                for (int j = 0 ; j < arrivals.count() ; j++)
                {
                    QVector<int> & str = double_sided_edges[arrivals[j].edge].truncated_str ;
                    if (!arrivals[j].str_inverted)
                        str.push_front(junctions[i].centers_indices[0]);
                    else
                        str.push_back(junctions[i].centers_indices[0]);


                }
            }

        }
        else
        {
            for (int j = 0 ; j < arrivals.count() ; j++)
            {
                QVector<int> &str = double_sided_edges[arrivals[j].edge].truncated_str ;
                if (!arrivals[j].str_inverted)
                    str.push_front(junctions[i].centers_indices[0]);
                else
                    str.push_back(junctions[i].centers_indices[0]);


            }
        }

    }

    //histo->display();
    //histo->saveToDisk();
    mainWindow->histoDoubleData << data ;

    QVector<int> fifo_in ;

    for (int i = 0 ; i < double_sided_edges.count() ; i++)
        fifo_in.append(i);

    while (fifo_in.count())
    {
        QVector<int> fifo_out ;
        for (int i = 0 ; i< fifo_in.count() ; i++)
        {
            int current = fifo_in[i] ;
            DoubleSidedEdge edge = double_sided_edges[current] ;
            if (junctions[edge.first_junction].equivalent.contains(current))
            {
                int next = junctions[edge.first_junction].equivalent.value(current) ;
                DoubleSidedEdge next_edge = double_sided_edges[next] ;
                if (edge.road_index<next_edge.road_index)
                {
                    next_edge.road_index = edge.road_index ;
                    double_sided_edges[next] = next_edge ;
                    fifo_out.append(next) ;
                }

            }
            if (junctions[edge.second_junction].equivalent.contains(current))
            {
                int next = junctions[edge.second_junction].equivalent.value(current) ;
                DoubleSidedEdge next_edge = double_sided_edges[next] ;
                if (edge.road_index<next_edge.road_index)
                {
                    next_edge.road_index = edge.road_index ;
                    double_sided_edges[next] = next_edge ;
                    fifo_out.append(next) ;
                }

            }
        }
        fifo_in = fifo_out ;
    }

    QHash<int,int> road_index;

    for (int i = 0 ; i < double_sided_edges.count() ; i++)
    {
        DoubleSidedEdge & edge = double_sided_edges[i] ;
        int index;
        if (road_index.contains(edge.road_index))
            index = road_index.value(edge.road_index) ;
        else
        {
            index = roads_junctions.count() ;
            road_index.insert(edge.road_index,index);
            roads_junctions.append(QSet<int>());
            roads_edges.append(QVector<int>());
        }

        roads_junctions[index].insert(edge.first_junction);
        roads_junctions[index].insert(edge.second_junction);
        junctions[edge.first_junction].roads.insert(index);
        junctions[edge.second_junction].roads.insert(index);
        roads_edges[index].append(i);
    }
    road_index.clear();

    //Gather histogram info for degree of junctions
    QVector<int> valid_junctions;
    QVector<int> degrees_of_junctions;
    QVector<int> second_degrees_of_junctions;
    for(int i = 0 ; i < junctions.count() ; i++)
    {
        int degree = junctions[i].arrivals.count() ;
        if ((degree==1)||(degree>=3))
        {
            valid_junctions<<second_degrees_of_junctions.count();
            degrees_of_junctions << degree ;
            int total = - degree ;
            for (int j = 0 ; j < degree ; j++)
            {
                Arrival & A = junctions[i].arrivals[j];
                DoubleSidedEdge & edge = double_sided_edges[A.edge] ;
                if (edge.first_junction==i)
                    total+=junctions[edge.second_junction].arrivals.count() ;
                else
                    total+=junctions[edge.first_junction].arrivals.count() ;
            }
            second_degrees_of_junctions<<total;
        }
        else valid_junctions<<-1;
    }

    QVector<int> third_degrees_of_junctions;
    for(int i = 0 ; i < junctions.count() ; i++)
    {

        if (valid_junctions[i]>-1){

            int total = - second_degrees_of_junctions[valid_junctions[i]] ;
            int degree = junctions[i].arrivals.count() ;
            for (int j = 0 ; j < degree ; j++)
            {
                Arrival & A = junctions[i].arrivals[j];
                DoubleSidedEdge & edge = double_sided_edges[A.edge] ;
                int second_degree_idx;
                if (edge.first_junction==i)
                    second_degree_idx  = valid_junctions[edge.second_junction];
                else
                    second_degree_idx  = valid_junctions[i];

                if (second_degree_idx>-1)
                    total+=second_degrees_of_junctions[second_degree_idx];

            }
            third_degrees_of_junctions<<total;
        }

    }

    /*IntHistogramLabel * degrees_of_junctions_histogram = new IntHistogramLabel(640,480,1,max_degree_of_junctions);
    for (int i = 0 ; i < degrees_of_junctions.count() ; i++)
        degrees_of_junctions_histogram->insertValue(degrees_of_junctions[i]);
    degrees_of_junctions_histogram->display();
*/
    mainWindow->histoIntData << degrees_of_junctions ;
    mainWindow->histoIntData << second_degrees_of_junctions ;
    mainWindow->histoIntData << third_degrees_of_junctions ;
    degrees_of_junctions.clear();
    second_degrees_of_junctions.clear();
    third_degrees_of_junctions.clear();

    //build line strings
    makeCurrent();


    pre_roads_index_vbo.clear();
    pre_roads_vbo.clear() ;
    pre_roads_index_vbo_start.clear();
    pre_roads_index_vbo_end.clear();
    mainWindow->roads_line_strings.clear();
    QVector<bool> edge_treated(double_sided_edges.count(),false) ;
    for (int m = 0 ; m < mainWindow->getRoadsJunctions().count() ; m++)
    {
        QVector<int> & edges = mainWindow->getRoadsEdges()[m];
        for (int l = 0 ; l < edges.count() ; l++)
            {

                int i = edges[l] ;
                DoubleSidedEdge edge =  double_sided_edges[i];

                if ((!edge_treated[i])&&
                        (((!junctions[edge.first_junction].equivalent.contains(i))||
                         (!junctions[edge.second_junction].equivalent.contains(i)))))
                {
                    edge_treated[i] = true ;
                    int prev = i , current ;
                    QVector<int> list ;
                    QVector<QPointF>  line_string;

                    if (junctions[edge.first_junction].equivalent.contains(prev))
                    {
                        current = junctions[edge.first_junction].equivalent.value(prev) ;
                        for (int j = edge.truncated_str.count()-1 ; j >=0 ; j--)
                            list << edge.truncated_str[j] ;
                    }
                    else if (junctions[edge.second_junction].equivalent.contains(prev))
                    {
                        current = junctions[edge.second_junction].equivalent.value(prev) ;
                        list << edge.truncated_str ;
                    }
                    else
                    {
                        current = -1 ;
                        list << edge.truncated_str ;
                    }

                    while (current>=0)
                    {
                        edge_treated[current] = true ;
                        edge = double_sided_edges[current] ;
                        if (junctions[edge.second_junction].equivalent.contains(prev))
                        {
                            for (int j = edge.truncated_str.count()-1 ; j >=0 ; j--)
                                list << edge.truncated_str[j] ;

                            if (junctions[edge.first_junction].equivalent.contains(current))
                            {
                                prev = current ;
                                current = junctions[edge.first_junction].equivalent.value(current) ;
                            }
                            else
                                current =-1 ;
                        }
                        else if (junctions[edge.first_junction].equivalent.contains(prev))
                        {
                            list << edge.truncated_str ;
                            if (junctions[edge.second_junction].equivalent.contains(current))
                            {
                                prev = current ;
                                current = junctions[edge.second_junction].equivalent.value(current) ;
                            }
                            else
                                current = -1 ;
                        }
                        else
                        {
                            qDebug() << "Bug1" ;
                            current = -1 ;
                        }
                    }//end while

                    float r = rand()/((float)RAND_MAX) ;
                    float g = rand()/((float)RAND_MAX) ;
                    float b = rand()/((float)RAND_MAX) ;
                    QColor c;
                    c.setRgbF(r,g,b);
                    mainWindow->roads_line_strings_colors << c ;

                    pre_roads_index_vbo_start << pre_roads_index_vbo.count() ;

                    int j = 0 ;
                    for (; j < list.count() -1 ; j++)
                    {
                        pre_roads_index_vbo << pre_roads_vbo.count()/6 << pre_roads_vbo.count()/6+1 ;
                        pre_roads_vbo << skel_vertices[list[j]].x() ;
                        pre_roads_vbo << skel_vertices[list[j]].y() ;
                        pre_roads_vbo << r ;
                        pre_roads_vbo << g ;
                        pre_roads_vbo << b ;
                        pre_roads_vbo << 1 ;
                        line_string << skel_vertices[list[j]];
                    }
                    //last element
                    if (list.count())
                    {
                        pre_roads_vbo << skel_vertices[list[j]].x() ;
                        pre_roads_vbo << skel_vertices[list[j]].y() ;
                        pre_roads_vbo << r ;
                        pre_roads_vbo << g ;
                        pre_roads_vbo << b ;
                        pre_roads_vbo << 1 ;
                        line_string << skel_vertices[list[j]];
                    }
                    pre_roads_index_vbo_end << pre_roads_index_vbo.count() ;
                    mainWindow->roads_line_strings << line_string ;

                }//end if !treated and start of line string


        }

    }//end for


    if (mainWindow->roads_vbo.isCreated())
        mainWindow->roads_vbo.destroy();

    mainWindow->roads_vbo.create() ;
    mainWindow->roads_vbo.bind() ;
    mainWindow->roads_vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
    mainWindow->roads_vbo.allocate(pre_roads_vbo.constData(),pre_roads_vbo.count()*sizeof(float));

    if (mainWindow->roads_index_vbo->isCreated())
        mainWindow->roads_index_vbo->destroy();

    mainWindow->roads_index_vbo->create() ;
    mainWindow->roads_index_vbo->bind() ;
    mainWindow->roads_index_vbo->setUsagePattern(QOpenGLBuffer::StaticDraw);
    mainWindow->roads_index_vbo->allocate(pre_roads_index_vbo.constData(),pre_roads_index_vbo.count()*sizeof(unsigned int));

    for (int i = 0 ; i < pre_roads_vbo.count() ; i+=6)
    {
        pre_roads_vbo[i+2] = 0 ;
        pre_roads_vbo[i+3] = 0 ;
        pre_roads_vbo[i+4] = 0 ;
    }

    if (mainWindow->black_roads_vbo.isCreated())
        mainWindow->black_roads_vbo.destroy();

    mainWindow->black_roads_vbo.create() ;
    mainWindow->black_roads_vbo.bind() ;
    mainWindow->black_roads_vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
    mainWindow->black_roads_vbo.allocate(pre_roads_vbo.constData(),pre_roads_vbo.count()*sizeof(GLfloat));

    QVector<unsigned int>  &pre_valid_roads_index_vbo = mainWindow->pre_valid_roads_index_vbo;


    QVector<int> &valid_roads = mainWindow->valid_roads;
    valid_roads.clear();
    fifo_in.clear() ;
    int * simple_distances = new int[(mainWindow->getRoadsJunctions().count()*(mainWindow->getRoadsJunctions().count()-1)/2)];
    int * tmp_distance = new int[mainWindow->getRoadsJunctions().count()];
    mainWindow->progress->setMaximum(mainWindow->getRoadsJunctions().count()-1);
    for (int i = 0 ; i < mainWindow->getRoadsJunctions().count() ; i++)
    {
        mainWindow->progress->setValue(i);
        for (int j = 0 ; j  < mainWindow->getRoadsJunctions().count() ; j++)
            if (i==j)
                tmp_distance[j] = 0 ;
            else
                tmp_distance[j] = INT_MAX;

        fifo_in.clear();
        fifo_in.append(i);
        while(fifo_in.count())
        {
            QVector<int> fifo_out;
            for (int j = 0 ; j < fifo_in.count() ; j++)
            {
                int index = fifo_in[j] ;
                QList<int> juncs = mainWindow->getRoadsJunctions()[index].toList() ;
                for (int k = 0 ; k < juncs.count() ; k++)
                {
                    int junc = juncs[k] ;
                    QList<int> roads = mainWindow->getJunctions()[junc].roads.toList() ;
                    for (int l = 0 ; l < roads.count() ; l++)
                        if (tmp_distance[index]+1<tmp_distance[roads[l]])
                        {
                            tmp_distance[roads[l]] = tmp_distance[index]+1 ;
                            fifo_out.append(roads[l]);
                        }
                }
            }
            fifo_in = fifo_out ;
        }


        int number_of_valid_distances = 0 ;
        for (int j = 0 ; j < mainWindow->getRoadsJunctions().count() ; j++)
        {
            if (j<i)
            {
                if (tmp_distance[j]<INT_MAX)
                    number_of_valid_distances++ ;
            }
            else
                if (i<j)
                {
                    simple_distances[(j*(j-1))/2+i] = tmp_distance[j] ;
                    if (tmp_distance[j]<INT_MAX)
                        number_of_valid_distances ++ ;
                }
        }
        if (number_of_valid_distances>mainWindow->getRoadsJunctions().count()/10)
            valid_roads << i ;

        qApp->processEvents();
    }

    for (int i = 0 ; i < valid_roads.count() ; i++ )
    {
        mainWindow->pre_valid_roads_index_vbo_start << pre_valid_roads_index_vbo.count() ;
        for (int j = pre_roads_index_vbo_start[valid_roads[i]] ;
             j < pre_roads_index_vbo_end[valid_roads[i]] ; j++)
        {

            pre_valid_roads_index_vbo << pre_roads_index_vbo[j] ;
        }
        mainWindow->pre_valid_roads_index_vbo_end << pre_valid_roads_index_vbo.count() ;

    }

    //closeness
    QVector<double> closeness;
    for (int i = 0 ; i < valid_roads.count() ; i++)
    {
        double close = 0 ;
        for (int j = 0 ; j < i ; j ++)
            close += simple_distances[(valid_roads[i]*(valid_roads[i]-1))/2+valid_roads[j]] ;
        for (int j = i+1 ; j < valid_roads.count() ; j ++)
            close += simple_distances[(valid_roads[j]*(valid_roads[j]-1))/2+valid_roads[i]] ;
        closeness << 1.0/close ;
    }
    mainWindow->histoDoubleData << closeness ;

    //degree , orthogonality
    QVector<int> degree;
    QVector<double> orthogonality ;
    for (int i = 0 ; i < valid_roads.count() ; i++)
    {
        int deg = 0 ;
        int total = 0 ;
        double orth = 0 ;
        QList<int> junctions = mainWindow->getRoadsJunctions()[valid_roads[i]].toList() ;
        for (int j = 0 ; j < junctions.count() ; j++)
        {
            QVector<Arrival> junction_arrivals = mainWindow->getJunctions()[junctions[j]].arrivals ;
            QVector<double> angles1,angles2;
            for (int k = 0 ; k < junction_arrivals.count() ; k++)
                if (!(mainWindow->getRoadsEdges()[valid_roads[i]].contains(junction_arrivals[k].edge)))
                {
                    deg++ ;
                    angles2.append(junction_arrivals[k].angle);
                }
                else
                    angles1.append(junction_arrivals[k].angle);
            for (int k = 0 ; k < angles1.count() ; k++)
                for (int l = 0 ; l < angles2.count() ; l++)
                {
                    orth += abs(sin((angles1[k]-angles2[l])*M_PI/180.0)) ;
                    total ++ ;
                }


        }

        degree << deg ;

        orth /= total ;
        orthogonality << orth ;
    }
    mainWindow->histoDoubleData << orthogonality ;
    orthogonality.clear();
    mainWindow->histoIntData << degree ;

    //spacing , length
    QVector<double> spacing ;
    QVector<double> length_roads_odo;
    QVector<int> length_roads_topo;
    QVector<double> length_edges;
    mainWindow->valid_roads_total_length = 0 ;
    for (int i = 0 ; i < valid_roads.count() ; i++)
    {

        double l = 0 ;
        int t = mainWindow->getRoadsEdges()[valid_roads[i]].count() ;
        for (int j = 0 ; j < t ; j++)
        {
            double l1 = mainWindow->getDoubleSidedEdges()[ mainWindow->getRoadsEdges()[valid_roads[i]][j]].length ;
            length_edges << l1 ;
            l += l1 ;
        }
        double s = l / degree[i] ;
        spacing << s ;
        length_roads_odo << l ;
        length_roads_topo << t ;
        mainWindow->valid_roads_total_length += l ;

    }
    mainWindow->histoDoubleData << spacing ;
    mainWindow->histoDoubleData << length_roads_odo ;
    mainWindow->histoDoubleData << length_edges ;
    mainWindow->histoIntData << length_roads_topo ;
    degree.clear();
    spacing.clear();
    length_roads_odo.clear();
    length_edges.clear();
    length_roads_topo.clear();


    QVector<float> circle;
    for (int i = 0 ; i < 40 ; i++)
    {
        float angle = M_PI*i/20.0f ;
        circle << cosf(angle) << sinf(angle) ;
    }


    pre_junctions_vbos.clear();
    mainWindow->junctions_contours_vbos.clear();
    mainWindow->junctions_bg_vbos.clear();

    for (int i = 0 ; i < junctions.count() ; i++)
    {

        std::vector<Point_2> circles ;
        QVector<QPointF> contour ;


        if (valid_junctions[i]>-1)
        {
            QVector<float> pre_junction_vbo ;
            for (int j = 0 ; j < junctions[i].centers_indices.count() ; j++)
            {
                float radius = skel_distance[junctions[i].centers_indices[j]]
                        * radiusFactor;
                float x = skel_vertices[junctions[i].centers_indices[j]].x() ;
                float y = skel_vertices[junctions[i].centers_indices[j]].y() ;

                for (int k = 0 ; k <= 40 ; k++)
                {
                    float xprime =  circle[(k%40)*2]*radius+x ;
                    float yprime = circle[(k%40)*2+1]*radius+y ;
                    circles.push_back(Point_2(xprime,yprime));

                }

            }


            std::vector<Point_2> hull(circles.size());
            std::vector<Point_2>::iterator ptr = CGAL::convex_hull_2
                    ( circles.begin(), circles.end(), hull.begin() );

            std::vector<Point_2>::iterator it;
            for (it = hull.begin(); it < ptr ; it++)
            {
                pre_junction_vbo <<(*it).x()<<(*it).y()
                                << 0 << 0 << 0 << 1  ;
                contour<< QPointF((*it).x(),(*it).y()) ;
            }
            pre_junction_vbo <<(*(hull.begin())).x()<<(*(hull.begin())).y()
                            << 0 << 0 << 0 << 1  ;
            contour<< QPointF((*hull.begin()).x(),(*hull.begin()).y()) ;

            pre_junctions_vbos << pre_junction_vbo ;
            mainWindow->places_contours_line_strings << contour ;
            QOpenGLBuffer *vbo = new QOpenGLBuffer;
            vbo->create();
            vbo->bind();
            vbo->setUsagePattern(QOpenGLBuffer::StaticDraw);
            vbo->allocate(pre_junction_vbo.constData(),pre_junction_vbo.count()*sizeof(float));
            mainWindow->junctions_contours_vbos << vbo ;

            for (int j = 0 ; j < pre_junction_vbo.count() ; j+=6)
            {
                pre_junction_vbo[j+2] = pre_junction_vbo[j+3] = pre_junction_vbo[j+4] = 1 ;
            }
            vbo = new QOpenGLBuffer;
            vbo->create();
            vbo->bind();
            vbo->setUsagePattern(QOpenGLBuffer::StaticDraw);
            vbo->allocate(pre_junction_vbo.constData(),pre_junction_vbo.count()*sizeof(float));
            mainWindow->junctions_bg_vbos << vbo ;

        }
    }
    doneCurrent();
((Page*)parent())->initDone = true ;
    update() ;
}


void RoadsWidget::exploreGraph(int current,int pred,QVector<int> & mark)
{
    Junction &junction = junctions[current] ;
    for (int j = 0 ; j < junction.arrivals.count() ; j++)
    {
        Arrival &arrival = junction.arrivals[j] ;
        DoubleSidedEdge &edge = double_sided_edges[arrival.edge] ;
        int second_junction_idx ;
        if (edge.first_junction==current)
            second_junction_idx = edge.second_junction ;
        else
            second_junction_idx = edge.first_junction ;

        if (second_junction_idx==pred)
            continue ;

        if (mark[second_junction_idx]>=0)
        {
            //found a crircuit
            qDebug() << "found a circuit of length " << mark[current] - mark[second_junction_idx] ;
        }
        else
        {
            mark[second_junction_idx] = mark[current] +1;
            exploreGraph(second_junction_idx,current , mark);
        }
    }

}


void RoadsWidget::explorePoint(int i)
{
    if (!index_junction.contains(i))
    {
        Junction junction;
        junction.mean_radius = skel_distance[i];
        junction.color = randColor() ;
        junction.centers_indices.append(i) ;
        junctions.append(junction) ;
        index_junction.insert(i,junctions.count()-1);
    }
    for (int j = 0 ; j < skel_children[i].count() ; j++)
        exploreEdge(i,skel_children[i][j]);

}

void RoadsWidget::exploreEdge( int first, int second)
{
    int current = second ;
    int prev = first ;
    int next;
    QPointF p0 = skel_vertices[prev]  , p1 = p0 ;
    QVector<int> str,truncated_str;
    double cumulated_distance = 0 ;
    str<<prev;
    QPointF p2;
    while (skel_children[current].count()==2)
    {
        str<<current;
        p2 = skel_vertices[current];
        QLineF p1p2(p1,p2);
        cumulated_distance += p1p2.length() ;
        next = (skel_children[current][0]==prev)?skel_children[current][1]:skel_children[current][0];
        prev = current;
        current = next ;
        p1=p2;
    }


    int last = current ;

    if (last<first)
    {
        str << last ;
        QPointF p3= skel_vertices[last];
        QLineF p0p3(p0,p3);
        double euclidean_distance = p0p3.length() ;
        double totalA = 0,totalB = 0;
        int total_width=0;
        double mean_angleA = 0 ;
        double mean_angleB = 0 ;
        double mean_width = 0 ;
        double d1A=0,d2A,d1B=euclidean_distance,d2B;

        if (euclidean_distance>(skel_distance[first]+skel_distance[last])*radiusFactor)
        {
            prev = first ;
            current = second ;
            p1 = p0 ;
            bool was_in_partA = false , was_in_partB = false ;
            double first_width,second_width ;
            QPointF first_point_A ,first_point_B  ;
            while (skel_children[current].count()==2)
            {
                p2 = skel_vertices[current];
                QLineF p1p2(p1,p2);
                double angle = p1p2.angle();
                QLineF p0p2(p0,p2);
                d2A = p0p2.length() ;
                if ((d2A>skel_distance[first]*radiusFactor)&&
                        (d1A<2*skel_distance[first]*radiusFactor)&&
                        (d1A<euclidean_distance-skel_distance[last]*radiusFactor))

                {
                    double delta = d2A - d1A ;
                    if (totalA>0)
                    {
                        if ((mean_angleA/totalA)-angle>180)
                            angle += 360 ;
                        else
                            if ((mean_angleA/totalA)-angle<-180)
                                angle -= 360 ;
                    }
                    mean_angleA += delta * angle ;
                    totalA += delta ;

                    if (was_in_partA==false)
                    {
                        first_width = skel_distance[current] ;
                        first_point_A = p2 ;
                    }
                    was_in_partA = true ;
                }

                d1A = d2A ;
                QLineF p2p3(p2,p3);
                d2B = p2p3.length() ;
                if ((d1B>skel_distance[last]*radiusFactor)&&
                        (d2B<2*skel_distance[last]*radiusFactor)&&
                        (d2B<euclidean_distance-skel_distance[first]*radiusFactor))
                {
                    double delta = d1B - d2B ;
                    if (totalB>0)
                    {
                        if ((mean_angleB/totalB)-angle>180)
                            angle += 360 ;
                        else
                            if ((mean_angleB/totalB)-angle<-180)
                                angle -= 360 ;
                    }
                    mean_angleB += delta * angle ;
                    totalB += delta ;

                    was_in_partB = true ;
                }
                else
                {
                    if (was_in_partB)
                    {
                        second_width = skel_distance[prev] ;
                        first_point_B = p1 ;
                    }
                    was_in_partB = false ;
                }

                if ((d2A>skel_distance[first]*radiusFactor)&&
                        (d1B>skel_distance[last]*radiusFactor))
                {
                    truncated_str << prev ;
                    mean_width+= skel_distance[prev] ;
                    total_width++;
                }

                d1B = d2B ;
                next = (skel_children[current][0]==prev)?skel_children[current][1]:skel_children[current][0];
                prev = current;
                current = next ;
                p1=p2;
            }


            mean_angleA /= totalA ;
            mean_angleB /= totalB ;
            mean_angleB -= 180 ;
            mean_width /= total_width;

            if((isnan(mean_angleA))||(isnan(mean_angleB)))
            {
                //if ((totalA==0)||(totalB==0)) qDebug() << "0 total in explore edge" ;
                //if (isnan(mean_angleA)||isnan(mean_angleB))

                qDebug() << "nan in explore edge" ;
                QLineF AB(skel_vertices[first], skel_vertices[last]);
                mean_angleA = AB.angle() ;

                mean_angleB = mean_angleA-180;
                truncated_str.clear() ;
                truncated_str << first << last ;

                first_point_A = skel_vertices[first] ;
                first_point_B = skel_vertices[last] ;

                mean_width = (first_width+second_width)*.5f;


            }

            DoubleSidedEdge edge ;
            edge.color = randColor() ;
            edge.first_index = first ;
            edge.second_index = last ;
            edge.length = cumulated_distance ;
            edge.first_width = first_width ;
            edge.second_width = second_width ;
            edge.road_index = double_sided_edges.count() ;
            edge.str = str ;
            edge.truncated_str = truncated_str ;
            edge.mean_distance = mean_width ;
            double_sided_edges.append(edge);

            Arrival arrival;
            arrival.angle = mean_angleB ;
            arrival.edge = double_sided_edges.count()-1 ;
            arrival.point = first_point_B ;
            arrival.str_inverted = true ;

            int junction_idx = index_junction[last] ;
            Junction junction = junctions[junction_idx] ;
            QVector<Arrival> arrivals = junction.arrivals ;
            arrivals.append(arrival);
            junction.arrivals = arrivals ;
            junctions[junction_idx] = junction ;

            junction_idx = index_junction[first] ;
            arrival.angle = mean_angleA ;
            arrival.edge = double_sided_edges.count()-1 ;
            arrival.point = first_point_A ;
            arrival.str_inverted = false ;

            junction = junctions[junction_idx] ;
            arrivals = junction.arrivals ;
            arrivals.append(arrival);
            junction.arrivals = arrivals ;
            junctions[junction_idx] = junction ;

        }
        else
        {//d<r1+r2 -> fusion
            if (index_junction.contains(last))
            {
                int  first_junction = index_junction.value(first) ;
                int  second_junction = index_junction.value(last) ;
                Junction junction = junctions[second_junction] ;
                Junction junction2 = junctions[first_junction] ;
                junction.mean_radius = (junction.mean_radius*junction.centers_indices.count()+
                                        junction2.mean_radius*junction2.centers_indices.count())/
                        (junction.centers_indices.count()+junction2.centers_indices.count()) ;
                for (int j = 0 ; j < junction2.centers_indices.count() ; j++)
                    index_junction.insert(junction2.centers_indices[j], second_junction );

                for (int i = 0 ; i < junction2.centers_indices.count() ; i++ )
                    junction.centers_indices.append(junction2.centers_indices[i]) ;

                for (int i = 0 ; i < junction2.arrivals.count() ; i++ )
                    junction.arrivals.append(junction2.arrivals[i]) ;

                junction2.arrivals.clear() ;

                junctions[second_junction] = junction ;
                junctions[first_junction]= junction2 ;
                /*Junction junction2 = junctions[first_junction] ;
            junction2.vertices.clear() ;
            junction2.arrivals.clear() ;
            junctions[first_junction] = junction2 ;*/
            }
            else
            {
                qDebug() << "BUG3\n" ;

            }
        }
    }


}

QRgb RoadsWidget::randColor()
{
    /*
     * float angle = 2*M_PI*rand()/32768.0 ;
    return qRgb((unsigned char)(127*(1+sinf(angle))),
                (unsigned char)(127*(1+sinf(angle+2*M_PI/3))),
                (unsigned char)(127*(1+sinf(angle+4*M_PI/3)))
                ) ;
               */
    return qRgb((unsigned char)rand(),(unsigned char)rand(),(unsigned char)rand()) ;
}


