#include "roadswidget.h"
#include "mainwindow.h"
#include "inthistogramlabel.h"
#include "histogramlabel.h"
#include "halfedge.h"
#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QPainter>
#include <cstdlib>
#include <limits.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/ch_graham_andrew.h>
#include <CGAL/convex_hull_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 Point_2;

bool edge_pairs_compare (const EdgePair & left,const EdgePair & right) {return left.B <right.B ; }

bool elements_compare(const Element & left,const Element & right) {return left.angle<right.angle;}


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
    pre_junctions_vbos(mainWindow->pre_junctions_vbos),
    junctions_line_strings(mainWindow->junctions_line_strings),
    valid_junctions(mainWindow->valid_junctions),
    valid_roads(mainWindow->valid_roads)

{

}

void RoadsWidget::initializeGL()
{
    initializeOpenGLFunctions();

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    glLineWidth(5);
    glEnable(GL_LINE_SMOOTH);

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
    for (uint32_t i = 0 ; i < mainWindow->junctions_contours_vbos.count() ; i++)
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
    for (uint32_t i = 0 ; i < mainWindow->junctions_bg_vbos.count() ; i++)
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
    //junctions connexity
    //    for (uint32_t i = 0 ; i < mainWindow->junctions_vbos.count() ; i++)
    //    {
    //        mainWindow->junctions_vbos[i]->bind();
    //        mainWindow->line_program->setUniformValue("matrix", m);
    //        mainWindow->line_program->enableAttributeArray(PROGRAM_VERTEX_ATTRIBUTE);
    //        mainWindow->line_program->enableAttributeArray(PROGRAM_COLOR_ATTRIBUTE);
    //        mainWindow->line_program->setAttributeBuffer(PROGRAM_VERTEX_ATTRIBUTE, GL_FLOAT, 0, 2, 6 * sizeof(GLfloat));
    //        mainWindow->line_program->setAttributeBuffer(PROGRAM_COLOR_ATTRIBUTE, GL_FLOAT, 2 * sizeof(GLfloat), 4, 6 * sizeof(GLfloat));
    //        glDrawArrays(GL_LINE_STRIP,0,mainWindow->junctions_vbos[i]->size()/(6*sizeof(float)));
    //        mainWindow->junctions_vbos[i]->release();
    //    }
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
        glDrawElements(GL_LINES, mainWindow->roads_index_vbo->size()/sizeof(uint32_t), GL_UNSIGNED_INT,0);
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

void RoadsWidget::addBranch(QVector<QLineF> &tree,const Arrival & arrival)
{

    if (isnan(arrival.angle)) qDebug() << "NAN in addBranch" ;
    float radius = qMax(mainWindow->openedQImage.width(),mainWindow->openedQImage.height()) ;
    //                        qDebug() << "radius" << radius ;
    QLineF l=QLineF::fromPolar(radius,arrival.angle+180);
    l.translate(arrival.point);

    qreal min_d = INFINITY ;
    QPointF min_d_point;
    QPointF current_point ;
    for (uint32_t k = 0 ; k < tree.count() ; k++)
    {
        uint32_t result = l.intersects(tree[k],&current_point) ;
        if (result==0) qDebug() << " 0 " << arrival.angle << tree[k].angle();
        if (result==QLineF::BoundedIntersection)
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
        QVector<uint32_t> & str = double_sided_edges[arrival.edge].truncated_str ;
        if (!arrival.str_inverted)
            str.push_front(skel_vertices.count() -1);
        else
            str.push_back(skel_vertices.count() -1);

        QLineF r(l.p1(),min_d_point);
        tree << r ;
    }


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
    mainWindow->places_contours_line_strings.clear();

    valid_junctions.clear();
    valid_roads.clear();

    //Build junctions and edges
    exploreGraph();

    //compute junctions edges equivalencies
    matchJunctionsEdges(threshold_on_B);

    //Compute Edges Unique Road Index
    computeEdgesRoadIndex();


    //compute RoadsJunctions And RoadsEdges
    computeRoadsJunctionsAndEdges();


    //compute closeness by finite simple distance
    qDebug() << "computeCloseness" ;
    computeCloseness();

    //Gather histogram info for degree of junctions
    qDebug() << "computeDegreeOfJunctions" ;
    computeDegreeOfJunctions();
    qDebug() << "computeRoadsLineStrings" ;


    //build line strings
    computeRoadsLineStrings();


    //degree , orthogonality , spacing , length odo & topo
    qDebug() << "computeOtherIndices" ;
    computeOtherIndices();
    qDebug() << "computeJunctionsMeanRadius" ;

    computeJunctionsMeanRadius();
    qDebug() << "computeJunctionsLineStrings" ;

    computeJunctionsLineStrings();
    qDebug() << "computeJunctionsHulls" ;
    computeJunctionsHulls();

    computeFacesSurfaces();

    ((Page*)parent())->initDone = true ;
    update() ;
    qDebug() << "fin build roads" ;
}


void RoadsWidget::exploreGraph()
{
    mainWindow->progress->setMaximum(skel_children.count());

    int total_vertices = 0 ;
    for (int i = 0 ; i  < skel_children.count() ; i++)
    {
        if((skel_children[i].count()==1)||(skel_children[i].count()>=3))
        {
            explorePoint(i);
            mainWindow->progress->setValue(i);
            qApp->processEvents();
        }
    }
    for (int i = 0 ; i < double_sided_edges.count() ; i++)
    {
        DoubleSidedEdge &edge = double_sided_edges[i] ;
        edge.first_junction = index_junction.value(edge.first_index);
        edge.second_junction = index_junction.value(edge.second_index);

    }
}


void RoadsWidget::explorePoint(uint32_t i)
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
    for (uint32_t j = 0 ; j < skel_children[i].count() ; j++)
        exploreEdge(i,skel_children[i][j]);

}

void RoadsWidget::computeJunctionsHulls()
{

    QVector<float> circle;
    for (uint32_t i = 0 ; i < 40 ; i++)
    {
        float angle = M_PI*i/20.0f ;
        circle << cosf(angle) << sinf(angle) ;
    }


    pre_junctions_vbos.clear();
    mainWindow->junctions_contours_vbos.clear();
    mainWindow->junctions_bg_vbos.clear();

    for (uint32_t i = 0 ; i < junctions.count() ; i++)
    {

        std::vector<Point_2> circles ;
        QVector<QPointF> contour ;

        //push arrivals points
        for (uint32_t j = 0 ; j < junctions[i].arrivals.count() ; j++)
        {
            QPointF & p = junctions[i].arrivals[j].point ;
            circles.push_back(Point_2(p.x(),p.y()));
        }

        if (valid_junctions[i]>-1)
        {
            QVector<float> pre_junction_vbo ;
            for (uint32_t j = 0 ; j < junctions[i].centers_indices.count() ; j++)
            {
                float radius = skel_distance[junctions[i].centers_indices[j]]
                        * radiusFactor;
                float x = skel_vertices[junctions[i].centers_indices[j]].x() ;
                float y = skel_vertices[junctions[i].centers_indices[j]].y() ;

                for (uint32_t k = 0 ; k <= 40 ; k++)
                {
                    float xprime =  circle[(k%40)*2]*radius+x ;
                    float yprime = circle[(k%40)*2+1]*radius+y ;
                    circles.push_back(Point_2(xprime,yprime));

                }

            }


            std::vector<Point_2> hull;
            CGAL::convex_hull_2
                    ( circles.begin(), circles.end(), std::back_inserter(hull) );

            std::vector<Point_2>::iterator it;
            for (it = hull.begin(); it !=hull.end() ; it++)
            {
                pre_junction_vbo <<(*it).x()<<(*it).y()
                                << 0 << 0 << 0 << 1  ;
                contour<< QPointF((*it).x(),(*it).y()) ;
            }
            pre_junction_vbo <<(*(hull.begin())).x()<<(*(hull.begin())).y()
                            << 0 << 0 << 0 << 1  ;
            contour<< QPointF((*(hull.begin())).x(),(*(hull.begin())).y()) ;

            pre_junctions_vbos << pre_junction_vbo ;
            mainWindow->places_contours_line_strings << contour ;
            QOpenGLBuffer *vbo = new QOpenGLBuffer;
            vbo->create();
            vbo->bind();
            vbo->setUsagePattern(QOpenGLBuffer::StaticDraw);
            vbo->allocate(pre_junction_vbo.constData(),pre_junction_vbo.count()*sizeof(float));
            vbo->release();
            mainWindow->junctions_contours_vbos << vbo ;

            for (uint32_t j = 0 ; j < pre_junction_vbo.count() ; j+=6)
            {
                pre_junction_vbo[j+2] = pre_junction_vbo[j+3] = pre_junction_vbo[j+4] = 1 ;
            }
            vbo = new QOpenGLBuffer;
            vbo->create();
            vbo->bind();
            vbo->setUsagePattern(QOpenGLBuffer::StaticDraw);
            vbo->allocate(pre_junction_vbo.constData(),pre_junction_vbo.count()*sizeof(float));
            vbo->release();
            mainWindow->junctions_bg_vbos << vbo ;

        }
    }
    doneCurrent();
}

void RoadsWidget::computeJunctionsLineStrings()
{
    for (uint32_t i = 0 ; i < junctions.count() ; i++)
    {
        if (junctions[i].arrivals.count())
        {
            QVector<QPointF> junctions_line_string ;
            QMap<float,uint32_t> elts;

            QVector<float> pre_junction_vbo;

            for (uint32_t j = 0 ; j < junctions[i].arrivals.count() ; j++)
            {
                QLineF l(skel_vertices[junctions[i].centers_indices[0]],junctions[i].arrivals[j].point);
                elts.insert( l.angle(),j);
            }

            QList<uint32_t> sorted = elts.values() ;
            for (uint32_t j = 0 ; j < sorted.count() ; j++)
            {
                QPointF & p = junctions[i].arrivals[sorted[j]].point ;
                junctions_line_string << p ;
                pre_junction_vbo << p.x() << p.y() << 0 << 0 << 0 << 1 ;
            }
            QPointF & p = junctions[i].arrivals[sorted[0]].point ;
            junctions_line_string << p ;
            pre_junction_vbo << p.x() << p.y() << 0 << 0 << 0 << 1 ;

            junctions_line_strings << junctions_line_string ;

            QOpenGLBuffer *vbo = new QOpenGLBuffer;
            vbo->create();
            vbo->bind();
            vbo->setUsagePattern(QOpenGLBuffer::StaticDraw);
            vbo->allocate(pre_junction_vbo.constData(),pre_junction_vbo.count()*sizeof(float));
            mainWindow->junctions_vbos << vbo ;
            vbo->release();
        }
    }

}

void RoadsWidget::computeJunctionsMeanRadius()
{
    for (uint32_t i = 0 ; i < junctions.count() ; i++)
    {
        if (junctions[i].centers_indices.count())
        {
            junctions[i].mean_radius = 0 ;

            for (uint32_t j = 0 ; j < junctions[i].centers_indices.count() ; j++)
                junctions[i].mean_radius += skel_distance[junctions[i].centers_indices[j]] ;

            junctions[i].mean_radius /= junctions[i].centers_indices.count() ;
        }
    }
}

void RoadsWidget::computeCloseness()
{
    //QVector<uint32_t>  &pre_valid_roads_index_vbo = mainWindow->pre_valid_roads_index_vbo;
    QVector<double> closeness;

    QVector<uint32_t> fifo_in ;

    valid_roads.clear();
    simple_distances.clear();
   QVector<uint32_t> tmp_distance(mainWindow->getRoadsJunctions().count());
    mainWindow->progress->setMaximum(mainWindow->getRoadsJunctions().count()-1);
    for (uint32_t i = 0 ; i < mainWindow->getRoadsJunctions().count() ; i++)
    {
        mainWindow->progress->setValue(i);
        for (uint32_t j = 0 ; j  < mainWindow->getRoadsJunctions().count() ; j++)
            if (i==j)
                tmp_distance[j] = 0 ;
            else
                tmp_distance[j] = std::numeric_limits<uint32_t>().max();

        fifo_in.clear();
        fifo_in.append(i);
        while(fifo_in.count())
        {
            QVector<uint32_t> fifo_out;
            for (uint32_t j = 0 ; j < fifo_in.count() ; j++)
            {
                uint32_t index = fifo_in[j] ;
                QList<uint32_t> juncs = mainWindow->getRoadsJunctions()[index].values() ;
                for (uint32_t k = 0 ; k < juncs.count() ; k++)
                {
                    uint32_t junc = juncs[k] ;
                    QList<uint32_t> roads = mainWindow->getJunctions()[junc].roads.values() ;
                    for (uint32_t l = 0 ; l < roads.count() ; l++)
                        if (tmp_distance[index]+1<tmp_distance[roads[l]])
                        {
                            tmp_distance[roads[l]] = tmp_distance[index]+1 ;
                            fifo_out.append(roads[l]);
                        }
                }
            }
            fifo_in = fifo_out ;
        }

        QVector<uint32_t> tmp2_distance;
        uint32_t valid_distances = 0 ;
        for (uint32_t j = 0 ; j < mainWindow->getRoadsJunctions().count() ; j++)
        {

                if (tmp_distance[j]<std::numeric_limits<uint32_t>().max())
                {
                    valid_distances++;
                   tmp2_distance << tmp_distance[j] ;
                }
        }
        if (valid_distances>mainWindow->getRoadsJunctions().count()/10)
        {
            valid_roads << i ;

        simple_distances << tmp2_distance ;

        double close = 0 ;
        for (uint32_t j = 0 ; j <tmp2_distance.count(); j ++)
            close += tmp2_distance[j] ;
       closeness << 1.0/close ;
        }
        else {
            QSet<uint32_t> & junctionsIndex = mainWindow->getRoadsJunctions()[i];
            QVector<Junction> & junctions = mainWindow->getJunctions();
            QList<uint32_t> list = junctionsIndex.values();
            for (int j = 0 ; j < list.count() ; ++j)
            {
                junctions[list[j]].arrivals.clear();
            }
        }
        qApp->processEvents();
    }
    /*
    for (uint32_t i = 0 ; i < valid_roads.count() ; i++ )
    {
        mainWindow->pre_valid_roads_index_vbo_start << pre_valid_roads_index_vbo.count() ;
        for (uint32_t j = pre_roads_index_vbo_start[valid_roads[i]] ;
             j < pre_roads_index_vbo_end[valid_roads[i]] ; j++)
        {

            pre_valid_roads_index_vbo << pre_roads_index_vbo[j] ;
        }
        mainWindow->pre_valid_roads_index_vbo_end << pre_valid_roads_index_vbo.count() ;

    }
    */

    mainWindow->histoDoubleData << closeness ;

}

void RoadsWidget::computeOtherIndices()
{
    QVector<uint32_t> degree;
    QVector<double> orthogonality ;
    for (uint32_t i = 0 ; i < valid_roads.count() ; i++)
    {
        uint32_t deg = 0 ;
        uint32_t total = 0 ;
        double orth = 0 ;
        QList<uint32_t> junctions = mainWindow->getRoadsJunctions()[valid_roads[i]].values() ;
        for (uint32_t j = 0 ; j < junctions.count() ; j++)
        {
            QVector<Arrival> junction_arrivals = mainWindow->getJunctions()[junctions[j]].arrivals ;
            QVector<double> angles1,angles2;
            for (uint32_t k = 0 ; k < junction_arrivals.count() ; k++)
                if (!(mainWindow->getRoadsEdges()[valid_roads[i]].contains(junction_arrivals[k].edge)))
                {
                    deg++ ;
                    angles2.append(junction_arrivals[k].angle);
                }
                else
                    angles1.append(junction_arrivals[k].angle);
            for (uint32_t k = 0 ; k < angles1.count() ; k++)
                for (uint32_t l = 0 ; l < angles2.count() ; l++)
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
    QVector<uint32_t> length_roads_topo;
    QVector<double> length_edges;
    mainWindow->valid_roads_total_length = 0 ;
    for (uint32_t i = 0 ; i < valid_roads.count() ; i++)
    {

        double l = 0 ;
        uint32_t t = mainWindow->getRoadsEdges()[valid_roads[i]].count() ;
        for (uint32_t j = 0 ; j < t ; j++)
        {
            double l1 = mainWindow->getDoubleSidedEdges()[ mainWindow->getRoadsEdges()[valid_roads[i]][j]].length ;
            if (settings.value("Crop/Unit",false).toBool())
                l1 *= settings.value("CropForm-UnitX",1.0).toDouble()/mainWindow->getOpenedImage()->width();
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
}

void RoadsWidget::computeRoadsLineStrings()
{
    makeCurrent();


    pre_roads_index_vbo.clear();
    pre_roads_vbo.clear() ;
    pre_roads_index_vbo_start.clear();
    pre_roads_index_vbo_end.clear();
    mainWindow->roads_line_strings.clear();
    QVector<bool> edge_treated(double_sided_edges.count(),false) ;
    QVector<uint32_t> list ;

    const uint32_t ROADS_NO_EDGE =(-1);

    for (uint32_t m = 0 ; m < valid_roads.count() ; m++)
    {
        QVector<uint32_t> & edges = mainWindow->getRoadsEdges()[valid_roads[m]];

        QVector<uint32_t> list;

        QVector<QPointF>  line_string;

        uint32_t startEdgeIdx = edges[0] ;
        DoubleSidedEdge startEdge =  double_sided_edges[startEdgeIdx];

        list << startEdge.truncated_str;

        edge_treated[startEdgeIdx] = true ;

        uint32_t lastJunctionIdx = startEdge.second_junction ;

        uint32_t currentEdgeIdx = junctions[lastJunctionIdx].equivalent.value(startEdgeIdx,ROADS_NO_EDGE);

        while (currentEdgeIdx!=ROADS_NO_EDGE)
        {
            if (!edge_treated[currentEdgeIdx])
            {
                edge_treated[currentEdgeIdx] = true ;
                if (lastJunctionIdx==double_sided_edges[currentEdgeIdx].first_junction)
                {
                    //add to line string double_sided_edges[currentEdgeIdx].truncated_str
                    list << double_sided_edges[currentEdgeIdx].truncated_str;

                    lastJunctionIdx = double_sided_edges[currentEdgeIdx].second_junction;

                }
                else
                {
                    //add to line string double_sided_edges[currentEdgeIdx].truncated_str inverted
                    for (int32_t k = double_sided_edges[currentEdgeIdx].truncated_str.count()-1;k>=0;k--)
                        list << double_sided_edges[currentEdgeIdx].truncated_str[k];

                    lastJunctionIdx = double_sided_edges[currentEdgeIdx].first_junction;

                }
                currentEdgeIdx = junctions[lastJunctionIdx].equivalent.value(currentEdgeIdx,ROADS_NO_EDGE);

            }
            else currentEdgeIdx = ROADS_NO_EDGE ;
        }

        lastJunctionIdx = startEdge.first_junction ;

        currentEdgeIdx = junctions[lastJunctionIdx].equivalent.value(startEdgeIdx,ROADS_NO_EDGE);

        while (currentEdgeIdx!=ROADS_NO_EDGE)
        {
            if (!edge_treated[currentEdgeIdx])
            {
                edge_treated[currentEdgeIdx] = true ;
                if (lastJunctionIdx==double_sided_edges[currentEdgeIdx].first_junction)
                {
                    //prepend to line string double_sided_edges[currentEdgeIdx].truncated_str inverted
                    QVector<uint32_t> tmp;
                    for (int k = double_sided_edges[currentEdgeIdx].truncated_str.count()-1;k>=0;k--)
                        tmp << double_sided_edges[currentEdgeIdx].truncated_str[k];

                    tmp << list ;
                    list = tmp ;

                    lastJunctionIdx = double_sided_edges[currentEdgeIdx].second_junction;

                }
                else
                {
                    //prepend to line string double_sided_edges[currentEdgeIdx].truncated_str
                    QVector<uint32_t> tmp =  double_sided_edges[currentEdgeIdx].truncated_str ;
                    tmp << list ;
                    list = tmp ;

                    lastJunctionIdx = double_sided_edges[currentEdgeIdx].first_junction;

                }
                currentEdgeIdx = junctions[lastJunctionIdx].equivalent.value(currentEdgeIdx,ROADS_NO_EDGE);

            }
            else currentEdgeIdx = ROADS_NO_EDGE ;
        }



        float r = rand()/((float)RAND_MAX) ;
        float g = rand()/((float)RAND_MAX) ;
        float b = rand()/((float)RAND_MAX) ;
        QColor c;
        c.setRgbF(r,g,b);
        mainWindow->roads_line_strings_colors << c ;

        pre_roads_index_vbo_start << pre_roads_index_vbo.count() ;

        uint32_t j = 0 ;
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

    }//end for road in roads

    qDebug() << "flag5" ;


    if (mainWindow->roads_vbo.isCreated())
        mainWindow->roads_vbo.destroy();

    mainWindow->roads_vbo.create() ;
    mainWindow->roads_vbo.bind() ;
    mainWindow->roads_vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
    mainWindow->roads_vbo.allocate(pre_roads_vbo.constData(),pre_roads_vbo.count()*sizeof(float));
    mainWindow->roads_vbo.release();

    if (mainWindow->roads_index_vbo->isCreated())
        mainWindow->roads_index_vbo->destroy();

    mainWindow->roads_index_vbo->create() ;
    mainWindow->roads_index_vbo->bind() ;
    mainWindow->roads_index_vbo->setUsagePattern(QOpenGLBuffer::StaticDraw);
    mainWindow->roads_index_vbo->allocate(pre_roads_index_vbo.constData(),pre_roads_index_vbo.count()*sizeof(uint32_t));
    mainWindow->roads_index_vbo->release() ;

    for (uint32_t i = 0 ; i < pre_roads_vbo.count() ; i+=6)
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
    mainWindow->black_roads_vbo.release();
}

void RoadsWidget::computeDegreeOfJunctions()
{
    QVector<uint32_t> degrees_of_junctions;
    QVector<uint32_t> second_degrees_of_junctions;
    for(uint32_t i = 0 ; i < junctions.count() ; i++)
    {
        uint32_t degree = junctions[i].arrivals.count() ;
        for (int j = 0 ; j < junctions[i].centers_indices.count() ; ++j)
        if (mainWindow->getSkelPointIsExit()[junctions[i].centers_indices[j]])
        {
            degree = 0 ;
            break;
        }
        if (degree){
            valid_junctions<<second_degrees_of_junctions.count();
            degrees_of_junctions << degree ;
            int total = - degree ;
            for (uint32_t j = 0 ; j < degree ; j++)
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

    qDebug() << "flag3" ;

    QVector<uint32_t> third_degrees_of_junctions;
    for(uint32_t i = 0 ; i < junctions.count() ; i++)
    {

        if (valid_junctions[i]>-1){

            int total = - second_degrees_of_junctions[valid_junctions[i]] ;
            uint32_t degree = junctions[i].arrivals.count() ;
            for (uint32_t j = 0 ; j < degree ; j++)
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


    mainWindow->histoIntData << degrees_of_junctions ;
    mainWindow->histoIntData << second_degrees_of_junctions ;
    mainWindow->histoIntData << third_degrees_of_junctions ;

}

void RoadsWidget::computeRoadsJunctionsAndEdges()
{
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
            roads_junctions.append(QSet<uint32_t>());
            roads_edges.append(QVector<uint32_t>());
        }

        roads_junctions[index].insert(edge.first_junction);
        roads_junctions[index].insert(edge.second_junction);
        junctions[edge.first_junction].roads.insert(index);
        junctions[edge.second_junction].roads.insert(index);
        roads_edges[index].append(i);
    }

}

void RoadsWidget::computeEdgesRoadIndex()
{
    QVector<uint32_t> fifo_in ;

    for (uint32_t i = 0 ; i < double_sided_edges.count() ; i++)
        fifo_in.append(i);

    while (fifo_in.count())
    {
        QVector<uint32_t> fifo_out ;
        for (uint32_t i = 0 ; i< fifo_in.count() ; i++)
        {
            uint32_t current = fifo_in[i] ;
            DoubleSidedEdge & edge = double_sided_edges[current] ;
            if (junctions[edge.first_junction].equivalent.contains(current))
            {
                uint32_t next = junctions[edge.first_junction].equivalent.value(current) ;
                DoubleSidedEdge & next_edge = double_sided_edges[next] ;
                if (edge.road_index<next_edge.road_index)
                {
                    next_edge.road_index = edge.road_index ;
                    fifo_out.append(next) ;
                }

            }
            if (junctions[edge.second_junction].equivalent.contains(current))
            {
                uint32_t next = junctions[edge.second_junction].equivalent.value(current) ;
                DoubleSidedEdge & next_edge = double_sided_edges[next] ;
                if (edge.road_index<next_edge.road_index)
                {
                    next_edge.road_index = edge.road_index ;
                    fifo_out.append(next) ;
                }

            }
        }
        fifo_in = fifo_out ;
    }
}

void RoadsWidget::matchJunctionsEdges(float threshold_on_B)
{
    const float delta_width_constant = logf(2);
    QVector<double> data;
    for (uint32_t i = 0 ; i < junctions.count() ; i++)
    {
        QVector<EdgePair> edge_pairs;
        const QVector<Arrival> & arrivals = junctions[i].arrivals ;
        if (arrivals.count()>=2)
        {

            for (uint32_t j = 0 ; j < arrivals.count() - 1; j++)

                for (uint32_t k = j+1 ; k < arrivals.count() ; k++)
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
C=0;
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

                    }
                }

            if (edge_pairs.count())
            {
                QSet<uint32_t> already_used_edge;
                std::sort(edge_pairs.begin(),edge_pairs.end(),edge_pairs_compare);
                for (uint32_t j = 0 ; j < edge_pairs.count() ; j++)
                {
                    if ((!already_used_edge.contains(edge_pairs[j].i))&&
                            (!already_used_edge.contains(edge_pairs[j].j))&&
                            (edge_pairs[j].B<threshold_on_B))
                    {

                        already_used_edge.insert(edge_pairs[j].i);
                        already_used_edge.insert(edge_pairs[j].j);
                        junctions[i].equivalent.insert(edge_pairs[j].i , edge_pairs[j].j ) ;
                        junctions[i].equivalent.insert(edge_pairs[j].j , edge_pairs[j].i ) ;

                    }
                }
            }
        }
    }

    mainWindow->histoDoubleData << data ;

}

void RoadsWidget::exploreEdge( uint32_t first, uint32_t second)
{
    uint32_t current = second ;
    uint32_t prev = first ;
    uint32_t next;
    QPointF p0 = skel_vertices[prev]  , p1 = p0 ;
    QVector<uint32_t> str,truncated_str;
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


    uint32_t last = current ;

    if (last<first)
    {
        str << last ;
        QPointF p3= skel_vertices[last];
        QLineF p0p3(p0,p3);
        double euclidean_distance = p0p3.length() ;
        double mean_angleA = 0 ;
        double mean_angleB = 0 ;
        double mean_width = 0 ;


        if (euclidean_distance>(skel_distance[first]+skel_distance[last])*radiusFactor)
        {
            double radius = skel_distance[first]*radiusFactor;

            double first_width,second_width ;
            QPointF first_point_A =p0,first_point_B =p3 ;
            uint32_t first_index_A=0,first_index_B=str.count()-1;
            QPointF p1 = p0 ;
            for (uint32_t i = 0 ; i < str.count() -1; i++)
            {
                p1 = skel_vertices[str[i]] ;
                QPointF p1prime = skel_vertices[str[i+1]] ;
                double r1 = QLineF(p0,p1).length() ;
                double r2 = QLineF(p0,p1prime).length() ;
                if((r1<radius)&&(r2>=radius))
                {
                    QPointF point= p1+(p1prime-p1)*(radius-r1)/(r2-r1) ;
                    first_point_A = point ;
                    first_index_A = i+1 ;
                }

            }

            radius = skel_distance[last]*radiusFactor ;
            QPointF p2 = p3 ;
            for (uint32_t i = str.count() -1 ; i > 0  ; i--)
            {
                p2 = skel_vertices[str[i]] ;
                QPointF p2prime = skel_vertices[str[i-1]] ;
                double r1 = QLineF(p3,p2).length() ;
                double r2 = QLineF(p3,p2prime).length() ;
                if((r1<radius)&&(r2>=radius))
                {
                    first_point_B = p2+(p2prime-p2)*(radius-r1)/(r2-r1) ;
                    first_index_B = i-1;
                }

            }

            QLineF p1p2(first_point_A,first_point_B);

            double angle = p1p2.angle();


            mean_angleA = angle ;
            mean_angleB = angle - 180 ;

            truncated_str.clear() ;
            truncated_str << skel_vertices.count() ;
            skel_vertices << first_point_A ;

            for (uint32_t i = first_index_A ; i <= first_index_B ; i++ )
                truncated_str << str[i] ;

            truncated_str << skel_vertices.count() ;
            skel_vertices << first_point_B ;


            first_width = skel_distance[str[first_index_A]];
            second_width = skel_distance[str[first_index_B]];

            mean_width = (first_width+second_width)*.5f;

            DoubleSidedEdge edge ;
            edge.color = randColor() ;
            edge.first_index = first ;
            edge.second_index = last ;
            edge.first_junction = index_junction[first];
            edge.second_junction = index_junction[last];
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

            uint32_t junction_idx = index_junction[last] ;
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
                uint32_t  first_junction = index_junction.value(first) ;
                uint32_t  second_junction = index_junction.value(last) ;
                if (first_junction!=second_junction)
                {
                    Junction junction = junctions[second_junction] ;
                    Junction junction2 = junctions[first_junction] ;
                    junction.mean_radius = (junction.mean_radius*junction.centers_indices.count()+
                                            junction2.mean_radius*junction2.centers_indices.count())/
                            (junction.centers_indices.count()+junction2.centers_indices.count()) ;
                    for (uint32_t j = 0 ; j < junction2.centers_indices.count() ; j++)
                        index_junction.insert(junction2.centers_indices[j], second_junction );

                    for (uint32_t i = 0 ; i < junction2.centers_indices.count() ; i++ )
                        junction.centers_indices.append(junction2.centers_indices[i]) ;

                    for (uint32_t i = 0 ; i < junction2.arrivals.count() ; i++ )
                        junction.arrivals.append(junction2.arrivals[i]) ;

                    junction2.arrivals.clear() ;
/*
                    if (junction.arrivals.count()==2)
                    {
                        DoubleSidedEdge & d1=double_sided_edges[junction.arrivals[0].edge];
                        DoubleSidedEdge & d2=double_sided_edges[junction.arrivals[1].edge];
                        if (d1.first_junction==second_junction)
                        {
                            if (d2.first_junction==second_junction)
                            {
                                d1.first_junction = d2.second_junction;
                                for (int j = 0;j<d2.str.count();j++)
                                {
                                    d1.str.prepend(d2.str[j]);
                                }
                                Junction & junction3 = junctions[d2.second_junction];
                                for (int j = 0 ; j < junction3.arrivals.count() ; ++j)
                                {
                                    if (junction3.arrivals[j].edge == junction.arrivals[1].edge)
                                    {
                                        junction3.arrivals[j].edge = junction.arrivals[0].edge;
                                        junction3.arrivals[j].str_inverted = false;
                                                break;
                                    }
                                }
                            }
                            else
                            {
                                d1.first_junction = d2.first_junction;
                                for (int j = d2.str.count()-1;j>=0;j--)
                                {
                                    d1.str.prepend(d2.str[j]);
                                }
                                Junction & junction3 = junctions[d2.first_junction];
                                for (int j = 0 ; j < junction3.arrivals.count() ; ++j)
                                {
                                    if (junction3.arrivals[j].edge == junction.arrivals[1].edge)
                                    {
                                        junction3.arrivals[j].edge = junction.arrivals[0].edge;
                                        junction3.arrivals[j].str_inverted = false;
                                                break;
                                    }
                                }
                            }
                        }
                        else
                        {
                            if (d2.first_junction==second_junction)
                            {
                                d1.second_junction = d2.second_junction;
                                for (int j = 0;j<d2.str.count();j++)
                                {
                                    d1.str << d2.str[j];
                                }
                                Junction & junction3 = junctions[d2.second_junction];
                                for (int j = 0 ; j < junction3.arrivals.count() ; ++j)
                                {
                                    if (junction3.arrivals[j].edge == junction.arrivals[1].edge)
                                    {
                                        junction3.arrivals[j].edge = junction.arrivals[0].edge;
                                        junction3.arrivals[j].str_inverted = true;
                                                break;
                                    }
                                }
                            }
                            else
                            {
                                d1.second_junction = d2.first_junction;
                                for (int j = d2.str.count()-1;j>=0;j--)
                                {
                                    d1.str<< d2.str[j];
                                }
                                Junction & junction3 = junctions[d2.first_junction];
                                for (int j = 0 ; j < junction3.arrivals.count() ; ++j)
                                {
                                    if (junction3.arrivals[j].edge == junction.arrivals[1].edge)
                                    {
                                        junction3.arrivals[j].edge = junction.arrivals[0].edge;
                                        junction3.arrivals[j].str_inverted = true;
                                                break;
                                    }
                                }
                            }
                        }
                        junction.arrivals.clear();
                    }*/

                    junctions[second_junction] = junction ;
                    junctions[first_junction]= junction2 ;
                }
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

void RoadsWidget::saveDistanceMatrixCSV()
{
    QFileInfo file(settings.value("File").toString()) ;

    //roads
    QString path = tr("%1/Matrice des distances-%2.csv").arg(file.absoluteDir().absolutePath()).arg(file.baseName());

    QFile data(path);
    if (data.open(QFile::WriteOnly)) {
        QTextStream out(&data);

        for (int i = 0 ; i < mainWindow->valid_roads.count() ; i++)
        {
            for (int j = 0 ; j < mainWindow->valid_roads.count() ; j++)
            {
                out << simple_distances[i][j] << ";";
            }
            out << "\n" ;
        }

        data.close();
    }

}


RoadsWidget::Side RoadsWidget::onWhichSide(QPointF & point)
{
    if (point.x()<=0)
        return SideW;
    else if (point.y()<=0)
        return SideN;
    else if (point.x()>=mainWindow->openedQImage.width()-1)
        return SideE;
    else return SideS;
}

void RoadsWidget::computeFacesSurfaces()
{

    HalfEdge he;
    QMap<uint32_t,uint32_t> junctionToVertex;
    QMap<QPair<uint32_t,uint32_t>,uint32_t> junctionsToHalfedge;

    for (int i = 0 ; i < junctions.count() ; ++i)
    {
        if (junctions[i].arrivals.count())
        {
        Vertex v;
        junctionToVertex.insert(i,he.vertices.count());
        v.junction = i;
        v.exit = false;
        for (int j = 0 ; j < junctions[i].centers_indices.count() ; ++j)
            if (mainWindow->getSkelPointIsExit()[junctions[i].centers_indices[j]])
               {
                v.exit = true ;
                break ;
            }
        he.vertices << v;
        }
    }

    for (int i = 0 ; i < valid_roads.count() ; ++i)
    {
        QVector<uint32_t> & edges =  roads_edges[valid_roads[i]];
        for (int j = 0 ; j < edges.count() ; ++j)
        {
            DoubleSidedEdge & d = double_sided_edges[ edges[j]];
            //if ((valid_junctions[d.first_junction]!=-1)&&(valid_junctions[d.second_junction]!=-1))
            {
            Edge e1;
            for (int k = 0 ; k < d.str.count();++k)
            e1.str << skel_vertices[d.str[k]];
            e1.road = d.road_index ;
            e1.vertex = junctionToVertex[d.first_junction];
            Edge e2;
            for (int k = d.str.count()-1 ; k >=0 ;--k)
            e2.str << skel_vertices[d.str[k]];
            e2.road = d.road_index ;
            e2.vertex = junctionToVertex[d.second_junction];
            e1.opposite = he.edges.count()+1;
            e2.opposite = he.edges.count();
            junctionsToHalfedge.insert(QPair<uint32_t,uint32_t>(d.first_junction,d.second_junction),he.edges.count());
            junctionsToHalfedge.insert(QPair<uint32_t,uint32_t>(d.second_junction,d.first_junction),he.edges.count()+1);
            he.edges << e1;
            he.edges << e2;
            }
        }
    }



    for (int i = 0 ; i < he.vertices.count() ; ++i)
    {
        Vertex & v = he.vertices[i];
        Junction & first = junctions[v.junction];
        QPointF center(0,0);
        for (int j = 0 ; j < first.centers_indices.count() ;++j)
        {
            center += skel_vertices[first.centers_indices[j]];
        }
        center/=first.centers_indices.count();
        v.center=center;
        QVector<Element> elements;
        for (int j = 0 ; j < first.arrivals.count() ; ++j)
        {
            Element e;
            QPointF p = first.arrivals[j].point - center ;
            e.angle = atan2(p.y(),p.x());
            e.edge = first.arrivals[j].edge ;
            elements << e;
        }
        std::sort(elements.begin(),elements.end(),elements_compare);
        DoubleSidedEdge & first_edge = double_sided_edges[elements[0].edge];
//        v.edge = junctionsToHalfedge[QPair<uint32_t,uint32_t>(v.junction,
//                                                              (first_edge.first_junction==v.junction)?
//                                                                  first_edge.second_junction:
//                                                                  first_edge.first_junction)];
        for (int j = 0 ; j < elements.count() ; ++j)
        {
            uint32_t edge_index = elements[j].edge;
            DoubleSidedEdge & current_edge = double_sided_edges[edge_index];
            uint32_t current_incident_halfedge = junctionsToHalfedge[QPair<uint32_t,uint32_t>((current_edge.first_junction==v.junction)?
                                                                                         current_edge.second_junction:
                                                                                         current_edge.first_junction,v.junction)];
            DoubleSidedEdge & next_edge = double_sided_edges[elements[(j+1)%elements.count()].edge];
            uint32_t  next_excident_halfedge =junctionsToHalfedge[QPair<uint32_t,uint32_t>(v.junction,(next_edge.first_junction==v.junction)?
                                                                                             next_edge.second_junction:
                                                                                             next_edge.first_junction)];
            he.edges[current_incident_halfedge].next = next_excident_halfedge ;
        }

    }
    /*
{
        QImage image(mainWindow->openedQImage.size(),QImage::Format_ARGB32);
                QPainter painter(&image);
                image.fill(0xFFFFFFFF);
                painter.setBrush(QBrush(Qt::black));
            for (int i = 0 ; i < he.edges.count() ; ++i)
            {
                QPointF p1(he.vertices[he.edges[i].vertex].center);
                QPointF p2(he.vertices[he.edges[he.edges[i].opposite].vertex].center);
                QPointF p3 = p2 + QPointF((p2.y()-p1.y())*.1,(p1.x()-p2.x())*.1)+QPointF((p1.x()-p2.x())*.1,(p1.y()-p2.y())*.1);
                QPointF p4 = p2*.3333+p1*.6666;
                QPen pen;
                    if(he.edges[i].next<he.edges.count())
                             pen = QPen(Qt::black);
                else
                             pen = QPen(Qt::red);
                painter.setPen(pen);
                   painter.drawLine(p1,p2);
                   painter.drawLine(p2,p3);
                   //painter.drawText(p4,QString("%1").arg(i));
            }
            QFileInfo file(settings.value("File").toString()) ;

            QString path = tr("%1/he.jpg").arg(file.absoluteDir().absolutePath());

            image.save(path);
    }
*/
QVector<QPolygonF> faces;
QVector<QVector<int> > edges;
QVector<bool> edge_treated(he.edges.count(),false);
    for (int i = 0 ; i < he.edges.count() ; ++i)
    {
                Vertex & v = he.vertices[he.edges[i].vertex];
                if (v.exit)
                {
                int e = i;
                do {
                    QVector<int> es;
                    int e2 = e ;
                    QPolygonF p;
                    do {
                        edge_treated[e2] = true ;
                        es << e2 ;
                        p << he.edges[e2].str;
                        e2 = he.edges[e2].next ;
                    } while (!he.vertices[he.edges[e2].vertex].exit);
                    /*
                    Side s1 = onWhichSide(p[0]);
                    Side s2 = onWhichSide(p[p.length()-1]);
                    if (s1!=s2)
                    {
                        if ((s1==SideN&&s2==SideW)||(s2==SideN&&s1==SideW))
                            p << QPoint(0,0);
                        else
                            if ((s1==SideN&&s2==SideE)||(s2==SideN&&s1==SideE))
                                p << QPoint(mainWindow->openedQImage.width()-1,0);
                            else
                                if ((s1==SideS&&s2==SideE)||(s2==SideS&&s1==SideE))
                                    p << QPoint(mainWindow->openedQImage.width()-1,mainWindow->openedQImage.height()-1);
                        else
                                    p << QPoint(0,mainWindow->openedQImage.height()-1);

                    }
                    */
                    faces << p;
                    edges << es ;
                    e = he.edges[ he.edges[e].opposite].next ;
                } while (e!=i);
            }
    }

int next_edge = edge_treated.indexOf(false);
while (next_edge!=-1)
{
    QVector<int> es;
    QPolygonF p;
    int e = next_edge;
    do {
        edge_treated[e]=true;
    p << he.edges[e].str;
es << e ;
e = he.edges[e].next;
    }while (e!=next_edge);
    edges << es ;
    faces << p ;
    next_edge = edge_treated.indexOf(false);
 }

    QVector<QPolygonF> polygons;
    for (int i = 0 ; i < mainWindow->getConnectedComponentsStarts().count() ; i++)
    {
        QPolygonF polygon;
        int p = mainWindow->getConnectedComponentsStarts()[i] ;
        do {
            QPointF & v = mainWindow->getContourVertices()[p] ;
            polygon<< v;
            p = mainWindow->getNextPointIndex()[p];
        } while (p!=mainWindow->getConnectedComponentsStarts()[i]) ;
       polygons << polygon ;

    }

    QVector<bool> nested(polygons.count(),false);
int nested_count = 0;
    for (int i  = 0 ; i < polygons.count() ; ++i)
        for (int j  = 0 ; j < polygons.count() ; ++j)
        {
            if (i==j) continue;

            if (polygons[j].containsPoint(polygons[i][0],Qt::OddEvenFill))
            {
                nested[i] = true ;
                nested_count++;
                break ;
            }
        }
qDebug() << "Nested count " << nested_count ;

 QVector<bool> face_treated(faces.count(),false);
QVector<bool> polygon_treated(polygons.count(),false);

    QVector<double> surfaces;
    for (int i  = 0 ; i < polygons.count() ; ++i)
    {
        if (nested[i])
            continue ;

        double surface = 0 ;
        for (int j = 0 ;j < polygons[i].count()-1 ; j++)
            surface += -polygons[i][j].y() * polygons[i][j+1].x() +
                    polygons[i][j].x() *polygons[i][j+1].y() ;
        surface += -polygons[i][polygons[i].count()-1].y() * polygons[i][0].x() +
                polygons[i][polygons[i].count()-1].x() * polygons[i][0].y() ;
        surface = 0.5 * abs(surface) ;
        if (settings.value("Crop/Unit",false).toBool())
            surface *= (settings.value("CropForm-UnitX",1.0).toDouble()/mainWindow->getOpenedImage()->width())*
                    (settings.value("CropForm-UnitX",1.0).toDouble()/mainWindow->getOpenedImage()->width());

        surfaces << surface ;

        for (int j = 0 ; j < faces.count() ; ++j)
        {
            //if (face_treated[j]) continue;
            if (faces[j].intersects(polygons[i]))
            {
                polygon_treated[i]=true;
                face_treated[j] = true ;
                Face f;
                f.surface = surface;
                f.contour = faces[j] ;
                for (int k = 0 ; k < edges[j].count() ; ++k)
                {
                    he.edges[edges[j][k]].face = he.faces.count();
                }
                f.stream_order = 1 ;
                he.faces << f ;
                break;
            }
        }

    }

    QVector<HalfEdge*> hes;
he.verifyIntegrity();
    HalfEdge * he1 = &he;
    uint32_t N = 1 ;
    while (he1->edges.count()>0)
    {
    he1->verifyIntegrity();
    HalfEdge * he2 = NULL;
    bool something_happened = false;
    do {
        if (he2) delete he2;
        he2 = he1->suppressRoadsOfTopoLengthN(N,something_happened);
    N++;
    } while(!something_happened);
    he2->verifyIntegrity();
    hes << he2;
    he1=he2;
    QImage image(mainWindow->openedQImage.size(),QImage::Format_ARGB32);
            QPainter painter(&image);
            image.fill(0xFFFFFFFF);
            painter.setBrush(QBrush(Qt::black));
        for (int i = 0 ; i < he2->edges.count() ; ++i)
        {
            QPointF p1(he2->vertices[he2->edges[i].vertex].center);
            QPointF p2(he2->vertices[he2->edges[he2->edges[i].next].vertex].center);
            /*QPointF p3 = p2 + QPointF((p2.y()-p1.y())*.1,(p1.x()-p2.x())*.1)+QPointF((p1.x()-p2.x())*.1,(p1.y()-p2.y())*.1);
            QPointF p4 = p2*.3333+p1*.6666;
            QPen pen;
                if(he2->edges[i].next<he2->edges.count())
                         pen = QPen(Qt::black);
            else
                         pen = QPen(Qt::red);
            painter.setPen(pen);*/
               painter.drawLine(p1,p2);
               /*painter.drawLine(p2,p3);
               painter.drawText(p4,QString("%1").arg(he2->edges[i].road));*/
        }
        QTextOption option;
        option.setAlignment(Qt::AlignCenter);

        for (int i = 0 ; i < he2->faces.count() ; ++i)
        {
            QStringList strl;
            strl << QString("%1:").arg(i);
            for (int j = 0 ; j < he2->faces[i].childrens.count() ; ++j)
                strl << QString("%1").arg(he2->faces[i].childrens.values()[j]);
            painter.drawText(he2->faces[i].contour.boundingRect(),QString("%1").arg(strl.join('\n')),option);
        }
        QFileInfo file(settings.value("File").toString()) ;

        QString path = tr("%1/he-%2.jpg").arg(file.absoluteDir().absolutePath()).arg(N);

        image.save(path);
    }

{
        QImage image(mainWindow->openedQImage.size()*2.1,QImage::Format_ARGB32);
                QPainter painter(&image);
                image.fill(0xFFFFFFFF);
                painter.setBrush(QBrush(Qt::black));
                for (int i = 0 ; i < he.edges.count() ; i+=2)
                {
                    QPointF p1(he.vertices[he.edges[i].vertex].center*2);
                    QPointF p2(he.vertices[he.edges[he.edges[i].next].vertex].center*2);
                    QPointF p3 = (p1+p2)*.5f;
                        painter.drawLine(p1,p2);
                        painter.drawText(p3,QString("%1,%2").arg(he.edges[i].face).arg(he.edges[he.edges[i].opposite].face));
                      }
                QTextOption option;
                option.setAlignment(Qt::AlignCenter);

                for (int i = 0 ; i < he.faces.count() ; ++i)
                {
                    painter.drawText(he.faces[i].contour.boundingRect().center()*2,QString("%1").arg(i));
                }
                QFileInfo file(settings.value("File").toString()) ;

                QString path = tr("%1/he-1.jpg").arg(file.absoluteDir().absolutePath());

                image.save(path);
    }

    QImage image(mainWindow->openedQImage.size(),QImage::Format_ARGB32);
    QPainter painter(&image);
    image.fill(0xFFFFFFFF);
    painter.setBrush(QBrush(Qt::black));
for (int i = 0 ; i < he.faces.count() ; ++i)
{
       painter.drawPolygon(he.faces[i].contour);
}

QFileInfo file(settings.value("File").toString()) ;

QString path = tr("%1/he-%2.jpg").arg(file.absoluteDir().absolutePath()).arg(file.baseName());

image.save(path);

    mainWindow->histoDoubleData << surfaces ;
}
