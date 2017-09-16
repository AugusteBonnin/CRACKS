#include "skelworker.h"
#include "customtypes.h"
#include "mainwindow.h"
#include "kdtree2d.h"
#include "skelpage.h"
#include <QApplication>

#include <QAction>

SkelWorker::SkelWorker(MainWindow * mainWindow) : QThread() ,
    mainWindow(mainWindow),
    connected_components(mainWindow->getConnectedComponentsStarts()) ,
    skel_children(mainWindow->getSkelChildren()) ,
    contour_vertices(mainWindow->getContourVertices()) ,
    skel_points(mainWindow->getSkelVertices()) ,
    next_point_index(mainWindow->getNextPointIndex()) ,
    skel_distance(mainWindow->getSkelDistanceToBoundary()) ,
    skel_colors(mainWindow->getSkelColors()) ,
    skel_indices(mainWindow->getSkelIndices())
{

}

int SkelWorker::getProgressMax()
{
    const int step1Max = connected_components.count();
    const int step2Max = mainWindow->cdt.number_of_vertices();
    return    step1Max+step2Max ;
}

SkelWorker::~SkelWorker()
{

}




void SkelWorker::crop()
{
    QPointF A(0,0);
    QPointF B(mainWindow->openedImage->width()-1,0);
    QPointF C(mainWindow->openedImage->width()-1,mainWindow->openedImage->height()-1);
    QPointF D(0,mainWindow->openedImage->height()-1);

    QLineF lines[] = {
        QLineF(A,B),QLineF(B,C),QLineF(C,D),QLineF(D,A)
    } ;

    QRectF frame(A,C);

    QVector<QVector<uint> > new_children(skel_children);
    QVector<QPointF> new_points(skel_points);


    for (int i = 0 ; i < skel_points.count() ; i++)
    {
        QPointF & p1 = new_points[i] ;
        for (int j = 0 ; j < skel_children[i].count() ; j++)
        {
            unsigned int i2 = skel_children[i][j] ;

            if (i<i2)
            {
                if (frame.contains(p1))
                {
                    QPointF & p2 = new_points[i2] ;

                    if (!frame.contains(p2))
                    {//p1 in frame , p2 not in frame
                        QPointF inter ;
                        QLineF p1p2(p1,p2) ;
                        for (int k = 0 ; k < 4 ; k++)
                            if (((p1p2.intersect(lines[k],&inter)==QLineF::BoundedIntersection)))
                            {
                                new_points << inter ;
                                skel_distance << INFINITY ;
                                QVector<unsigned int> succ;
                                succ << i;
                                new_children << succ ;
                                new_children[i].removeOne(i2);
                                new_children[i].append(new_points.count()-1);
                                new_children[i2].clear();
                                break ;
                            }



                    } //else do nothing (p1 & p2 are in frame)
                }
                else //p1 not in frame
                {
                    QPointF & p2 = new_points[i2] ;

                    if (frame.contains(p2))
                    {//p1 not in frame , p2 in frame
                        QPointF inter ;
                        QLineF p1p2(p1,p2) ;
                        for (int k = 0 ; k < 4 ; k++)
                            if (((p1p2.intersect(lines[k],&inter)==QLineF::BoundedIntersection)))
                            {
                                new_points << inter ;
                                skel_distance << INFINITY ;
                                QVector<unsigned int> succ;
                                succ << i2;
                                new_children << succ ;
                                new_children[i2].removeOne(i);
                                new_children[i2].append(new_points.count()-1);
                                new_children[i].clear();
                                break ;
                            }



                    }
                    else
                    {
                        //p1 & p2 not in frame
                        new_children[i2].clear() ;new_children[i].clear() ;
                    }
                }
            }
        }
    }

    skel_children = new_children ;
    skel_points = new_points ;
}


void
mark_domains(CDT& ct,
             CDT::Face_handle start,
             int index,
             std::list<CDT::Edge>& border )
{
    if(start->info().nesting_level != -1){
        return;
    }
    std::list<CDT::Face_handle> queue;
    queue.push_back(start);
    while(! queue.empty()){
        CDT::Face_handle fh = queue.front();
        queue.pop_front();
        if(fh->info().nesting_level == -1){
            fh->info().nesting_level = index;
            for(int i = 0; i < 3; i++){
                CDT::Edge e(fh,i);
                CDT::Face_handle n = fh->neighbor(i);
                if(n->info().nesting_level == -1){
                    if(ct.is_constrained(e)) border.push_back(e);
                    else queue.push_back(n);
                }
            }
        }
    }
}
//explore set of facets connected with non constrained edges,
//and attribute to each such set a nesting level.
//We start from facets incident to the infinite vertex, with a nesting
//level of 0. Then we recursively consider the non-explored facets incident
//to constrained edges bounding the former set and increase the nesting level by 1.
//Facets in the domain are those with an odd nesting level.
void
mark_domains(CDT& cdt)
{
    for(CDT::All_faces_iterator it = cdt.all_faces_begin(); it != cdt.all_faces_end(); ++it){
        it->info().nesting_level = -1;
    }
    std::list<CDT::Edge> border;
    mark_domains(cdt, cdt.infinite_face(), 0, border);
    while(! border.empty()){
        CDT::Edge e = border.front();
        border.pop_front();
        CDT::Face_handle n = e.first->neighbor(e.second);
        if(n->info().nesting_level == -1){
            mark_domains(cdt, n, e.first->info().nesting_level+1, border);
        }
    }
}


void SkelWorker::run()
{

    //Insert the polygons into a constrained triangulation
    Triangulation & cdt = mainWindow->cdt;
    cdt.clear();

    for (int i = 0 ; i < connected_components.count() ; i++)
    {
        Polygon_2 polygon1;
        int p = connected_components[i] ;
        do {
            QPointF & v = contour_vertices[p] ;
            polygon1.push_back(CGALPoint(v.x(),v.y()));
            p = next_point_index[p];
        } while (p!=connected_components[i]) ;
        cdt.insert_constraint(polygon1.vertices_begin(), polygon1.vertices_end(), true);

        emit progressIncrement(1) ;

    }

    //Mark facets that are inside the domain bounded by the polygon
    mark_domains(mainWindow->cdt);


    QMap<QPair<Vertex_handle,Vertex_handle>,int> middlesIdx;
    QMap<CGALTriangle,int> & centersIdx = mainWindow->centersIdx;
    centersIdx.clear();
    skel_points.clear();
    skel_children.clear() ;
    skel_distance.clear() ;

    mainWindow->point_cloud.handles.clear(); ;
    mainWindow->point_cloud.pts.clear(); ;
    mainWindow->treeIdx2Handle.clear(); ;


    for (Triangulation::Finite_vertices_iterator it = mainWindow->cdt.finite_vertices_begin() ; it!=mainWindow->cdt.finite_vertices_end() ; ++it)
    {

        KDTree2D::PointCloud::Point p;
        p.x = it->point().x() ;
        p.y = it->point().y() ;
        mainWindow->point_cloud.pts.push_back(p) ;
        mainWindow->treeIdx2Handle.push_back(it);

        Triangulation::Face_circulator start = mainWindow->cdt.incident_faces(it) ;
        Triangulation::Face_circulator current = start ;
        Triangulation::Face_circulator next = start ;
        --next ;

        do {
            if (!next->info().in_domain())
            {
                if ((!current->info().in_domain()))
                {
                    if ((!(cdt.is_infinite(next)))&&(!(cdt.is_infinite(current))))
                   {
                    int center_t1_idx ;
                    CGALTriangle t1(current->vertex(0),current->vertex(1),current->vertex(2));
                    if (centersIdx.contains(t1))
                        center_t1_idx = centersIdx.value(t1) ;
                    else
                    {
                        center_t1_idx = skel_points.count() ;
                        centersIdx[t1] = skel_points.count() ;
                        QPointF center = t1.center() ;
                        QPointF A(it->point().x(),it->point().y());
                        skel_distance.append(INFINITY) ;
                        skel_points <<  center ;
                        skel_children.append( QVector<unsigned int>()) ;
                    }

                    CGALTriangle t2(next->vertex(0),next->vertex(1),next->vertex(2));
                    unsigned int center_t2_idx ;
                    if (centersIdx.contains(t2))
                        center_t2_idx = centersIdx.value(t2) ;
                    else
                    {
                        center_t2_idx = skel_points.count() ;
                        centersIdx[t2] = skel_points.count() ;
                        QPointF center = t2.center() ;
                        QPointF A(it->point().x(),it->point().y());
                        skel_distance.append(INFINITY) ;
                        skel_points <<  center ;
                        skel_children.append( QVector<unsigned int>()) ;
                    }


                    skel_children[center_t1_idx].append( center_t2_idx );
                    }
                }
                else //current face in domain
                {
                    if ((!next->info().in_domain())&&(!cdt.is_infinite(next)))
                    {
                        int middle_idx ;
                        Vertex_handle p2 = current->vertex(cdt.ccw(current->index(it))) ;
                        QPair<Vertex_handle,Vertex_handle> pair(it,p2);
                        if (pair.first>pair.second)
                        {
                            Vertex_handle tmp = pair.first ;
                            pair.first = pair.second ;
                            pair.second = tmp ;
                        }
                        if (middlesIdx.contains(pair))
                        {
                            middle_idx = middlesIdx.value(pair) ;
                        }
                        else
                        {
                            QPointF A(it->point().x(),it->point().y());
                            QPointF B(p2->point().x(),p2->point().y());

                            QPointF middle = QLineF(A,B).pointAt(.5) ;
                            middle_idx = skel_points.count() ;
                            middlesIdx[pair] = skel_points.count() ;
                            skel_points << middle ;
                            if (isFramePoint(mainWindow->openedImage,middle))
                                skel_distance.append(INFINITY) ;
                            else
                                skel_distance.append(0) ;
                            skel_children.append( QVector<unsigned int>()) ;
                        }

                        CGALTriangle t1(next->vertex(0),next->vertex(1),next->vertex(2));
                        int center_t1_idx ;
                        if (centersIdx.contains(t1))
                            center_t1_idx = centersIdx.value(t1) ;
                        else
                        {
                            center_t1_idx = skel_points.count() ;
                            centersIdx[t1] = skel_points.count() ;
                            QPointF center = t1.center() ;
                            QPointF A(it->point().x(),it->point().y());
                            skel_distance.append(INFINITY) ;
                            skel_points <<  center ;
                            skel_children.append( QVector<unsigned int>()) ;
                        }
                        skel_children[middle_idx].append(center_t1_idx) ;
                    }


                }
            }
                else // next face in domain
                {
                if ((!current->info().in_domain())&&(!cdt.is_infinite(current)))
                {
                    int middle_idx ;
                    Vertex_handle p2 = current->vertex(cdt.ccw(current->index(it))) ;
                    QPair<Vertex_handle,Vertex_handle> pair(it,p2);
                    if (pair.first>pair.second)
                    {
                        Vertex_handle tmp = pair.first ;
                        pair.first = pair.second ;
                        pair.second = tmp ;
                    }
                    if (middlesIdx.contains(pair))
                    {
                        middle_idx = middlesIdx.value(pair) ;
                    }
                    else
                    {
                        QPointF A(it->point().x(),it->point().y());
                        QPointF B(p2->point().x(),p2->point().y());

                        QPointF middle = QLineF(A,B).pointAt(.5) ;
                        middle_idx = skel_points.count() ;
                        middlesIdx[pair] = skel_points.count() ;
                        skel_points << middle ;
                        if (isFramePoint(mainWindow->openedImage,middle))
                            skel_distance.append(INFINITY) ;
                        else
                            skel_distance.append(0) ;
                        skel_children.append( QVector<unsigned int>()) ;
                    }

                    CGALTriangle t1(current->vertex(0),current->vertex(1),current->vertex(2));
                    int center_t1_idx ;
                    if (centersIdx.contains(t1))
                        center_t1_idx = centersIdx.value(t1) ;
                    else
                    {
                        center_t1_idx = skel_points.count() ;
                        centersIdx[t1] = skel_points.count() ;
                        QPointF center = t1.center() ;
                        QPointF A(it->point().x(),it->point().y());
                        skel_distance.append(INFINITY) ;
                        skel_points <<  center ;
                        skel_children.append( QVector<unsigned int>()) ;
                    }
                    skel_children[center_t1_idx].append(middle_idx) ;
                }

            }

            current = next ;
            --next ;
        } while (current!=start) ;


        emit progressIncrement(1);

    }

    if (mainWindow->kdtree)
        delete mainWindow->kdtree ;
    mainWindow->kdtree = new KDTree2D::my_kd_tree_2d_type(2,mainWindow->point_cloud,KDTreeSingleIndexAdaptorParams(10)) ;

    mainWindow->kdtree->buildIndex();

    //cropping
    crop() ;

    //Renforcement
//    for (int i = 0 ; i < skel_children.count() ; i++)
//        for (int j = 0 ; j < skel_children[i].count(); j++)
//        {
//            int k = skel_children[i][j] ;
//            if (!skel_children[k].contains(i))
//                skel_children[k] << i ;
//        }


    //computing skel distance
    QVector<uint> file_in;
    for (int i = 0 ; i < skel_distance.count() ; i++)
        if (skel_distance[i]==0)
            file_in << i ;

    while (file_in.count())
    {
        qDebug() << file_in.count() ;
        QVector<uint> file_out;

        for (int i = 0 ; i < file_in.count() ; ++i)
        {
            uint i1 = file_in[i] ;
            QPointF & p1 = skel_points[i1];
            for (int j = 0 ; j < skel_children[i1].count() ; ++j)
            {
                uint i2 = skel_children[i1][j] ;
                QPointF & p2 = skel_points[i2];
                double d = skel_distance[i1]+QLineF(p1,p2).length() ;

                if (skel_distance[i2]>d)
                {
                    skel_distance[i2] = d ;
                    file_out << i2 ;
                }
            }
        }

        file_in = file_out ;

    }

    /*
                                                //prepare selection buffer
                                                QList<CGALTriangle> list = centersIdx.keys() ;
                                                QVector<float> vbo ;
                                                QVector<unsigned int> index_vbo ;
                                                for (int i = 0 ; i < list .count() ; i++)
                                                {
                                                    float r,g,b,a ;
                                                    r = (i&0xFF)/255.0f ;
                                                    g =  ((i>>8)&0xFF) / 255.0f ;
                                                    b = ((i>>16)&0xFF) / 255.0f  ;
                                                    a = ((i>>24)&0xFF) / 255.0f ;
                                                    vbo << list[i].vertices[0]->point().x() << list[i].vertices[0]->point().y() ;
                                                    vbo << r << g << b << a ;
                                                    index_vbo << index_vbo.count() ;

                                                    vbo << list[i].vertices[1]->point().x() << list[i].vertices[1]->point().y() ;
                                                    vbo << r << g << b << a ;
                                                    index_vbo << index_vbo.count() ;

                                                    vbo << list[i].vertices[2]->point().x() << list[i].vertices[2]->point().y() ;
                                                    vbo << r << g << b << a ;
                                                    index_vbo << index_vbo.count() ;
                                                }

                                                if (mainWindow->faces_index_vbo->isCreated())
                                                    mainWindow->faces_index_vbo->destroy();

                                                mainWindow->faces_index_vbo->create() ;
                                                mainWindow->faces_index_vbo->bind() ;
                                                mainWindow->faces_index_vbo->setUsagePattern(QOpenGLBuffer::StaticDraw);
                                                mainWindow->faces_index_vbo->allocate(index_vbo.constData(),index_vbo.count()*sizeof(unsigned int));

                                                if (mainWindow->faces_vbo.isCreated())
                                                    mainWindow->faces_vbo.destroy();

                                                mainWindow->faces_vbo.create() ;
                                                mainWindow->faces_vbo.bind();
                                                mainWindow->faces_vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
                                                mainWindow->faces_vbo.allocate(vbo.constData(),vbo.count()*sizeof(float));
                                            */
    /*
                                                //distance from bounds to center
                                                QVector<int> fifo_in ;

                                                for (int i = 0 ; i < skel_distance.count() ; i++)
                                                    if (skel_distance[i]==0)
                                                        fifo_in.append(i);

                                                while (fifo_in.count())
                                                {
                                                    QVector<int> fifo_out ;

                                                    for (int i = 0 ; i  < fifo_in.count() ; i++)
                                                    {
                                                        int index = fifo_in[i];

                                                        QPointF &p1 = skel_points[index];
                                                        for (int j = 0 ; j < skel_children[index].count() ; j++)
                                                        {
                                                            int i2 = skel_children[index][j] ;
                                                            QPointF &p2 = skel_points[i2] ;
                                                            double d = QLineF(p1,p2).length() + skel_distance[index] ;
                                                            if (skel_distance[i2]>d)
                                                            {
                                                                skel_distance[i2] = d ;
                                                                fifo_out.append(i2);
                                                            }
                                                        }
                                                    }
                                                    fifo_in = fifo_out ;
                                                }*/


    //pruning

    QVector<QVector<uint> > new_children(skel_children);
    bool something_happend = true ;

    while (something_happend)
    {
        something_happend = false ;

        for (int i = 0 ; i < skel_children.count() ; i++)
        {
            QVector<unsigned int> & v = skel_children[i] ;
            QPointF & point = skel_points[i] ;
            if ((v.count()==1)&&(!isFramePoint(mainWindow->openedImage,point)))
                //if (v.count()==1)
            {
                int current = v[0] ;
                int last = i ;
                int next;
                qreal length = 0 ;
                QPointF p1 = skel_points[last];
                QVector<int> str;
                str<<last;
                QVector<unsigned int> v2 = skel_children[current] ;

                while (v2.count()==2)
                {
                    str<<current;
                    QPointF p2 = skel_points[current];
                    QLineF line(p1,p2);
                    length  += line.length();
                    next = (v2[0]==last)?v2[1]:v2[0];
                    last = current;
                    current = next ;
                    p1=p2;
                    v2 = skel_children[current] ;
                }

                QPointF p2 = skel_points[current];
                QLineF line(p1,p2);
                length  += line.length();
                str << current ;

                QSettings settings;
                if (skel_distance[current]>=length*settings.value("Skel/Prune",2.0).toDouble())
                {
                    something_happend = true ;

                    if (new_children[str[str.count()-1]].indexOf(str[str.count()-2])>-1)
                        new_children[str[str.count()-1]].remove(new_children[str[str.count()-1]].indexOf(str[str.count()-2]));

                    for (int j = 1 ; j < str.count() - 1 ; j++)
                        new_children[str[j]].clear();

                    if (new_children[str[0]].indexOf(str[1])>-1)
                        new_children[str[0]].remove(new_children[str[0]].indexOf(str[1]));
                }
            }

        }
        skel_children = new_children ;



    }


}

