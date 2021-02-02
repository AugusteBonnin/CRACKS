#include "skelworker3.h"
#include "customtypes.h"
#include "mainwindow.h"
#include "kdtree2d.h"
#include "skelpage.h"
#include <QApplication>


SkelWorker3::SkelWorker3(MainWindow * mainWindow) : QThread() ,
    mainWindow(mainWindow),
    connected_components(mainWindow->getConnectedComponentsStarts()) ,
    skel_children(mainWindow->getSkelChildren()) ,
    contour_vertices(mainWindow->getContourVertices()) ,
    skel_points(mainWindow->getSkelVertices()) ,
    next_point_index(mainWindow->getNextPointIndex()) ,
    skel_distance(mainWindow->getSkelDistanceToBoundary()) ,
    skel_colors(mainWindow->getSkelColors()) ,
    skel_indices(mainWindow->getSkelIndices()),
    skel_point_is_exit(mainWindow->getSkelPointIsExit())
{

}

int SkelWorker3::getProgressMax()
{
    const int step1Max = connected_components.count();
    const int step2Max = mainWindow->cdt.number_of_vertices();
    return    step1Max+step2Max ;
}

SkelWorker3::~SkelWorker3()
{

}


int SkelWorker3::create_cendroid(const Vertex_handle& v0,const Vertex_handle & v1,const Vertex_handle & v2)
{
    QPointF inter;

    QPointF A(v0->point().x(),v0->point().y());
    QPointF B(v1->point().x(),v1->point().y());
    QPointF C(v2->point().x(),v2->point().y());

    QLineF AB(A,B) ;
    QLineF medAB = AB.normalVector() ;
    medAB.translate(AB.pointAt(.5)-A);

    QLineF AC(A,C) ;
    QLineF medAC = AC.normalVector() ;
    medAC.translate(AC.pointAt(.5)-A);

    medAB.intersect(medAC,&inter);
    int centroid_idx = skel_points.count();
    skel_points << inter ; skel_children<<QVector<uint>() ;

    double distance = QLineF(A,inter).length();
    skel_distance << distance ;
    skel_point_is_exit << false ;

    return centroid_idx ;
}
int SkelWorker3::create_cendroid(const Vertex_handle& v0,const Vertex_handle & v1,const Vertex_handle & v2,const Vertex_handle & v3)
{
    QPointF centroid((v0->point().x()+v1->point().x()+v2->point().x()+v3->point().x())*.25f,
                     (v0->point().y()+v1->point().y()+v2->point().y()+v3->point().y())*.25f);
    int centroid_idx = skel_points.count();
    skel_points << centroid ; skel_children<<QVector<uint>() ;

    QPointF p0(v0->point().x(),v0->point().y());
    QPointF p1(v1->point().x(),v1->point().y());
    QPointF p2(v2->point().x(),v2->point().y());
    double distance = qMin(QLineF(p0,centroid).length(),QLineF(p1,centroid).length());
    distance = qMin(distance,QLineF(p2,centroid).length());

    skel_distance << distance ;
    skel_point_is_exit << false ;

    return centroid_idx ;
}

int SkelWorker3::maibe_create_middle(const Vertex_handle & v1, const Vertex_handle & v2,bool point_is_exit)
{
    QPair<Vertex_handle,Vertex_handle> pair(v1,v2);
    if (v1>v2)
        pair = QPair<Vertex_handle,Vertex_handle>(v2,v1);
    int middle_idx = middles_idxs.value((pair),-1);
    static int calls = 0 ;
    calls++;
    qDebug() << "calls:" << calls ;
    if (-1==middle_idx)
    {
        middle_idx = skel_points.count() ;
        middles_idxs.insert(pair,middle_idx);

        QPointF middle((v1->point().x()+v2->point().x())*.5f,(v1->point().y()+v2->point().y())*.5f);

        skel_points << middle ;
        skel_children << QVector<unsigned int> () ;

        QPointF p1(v1->point().x(),v1->point().y());
        QLineF p1_middle(p1,middle);
        /*
         * if (point_is_exit)
            skel_distance << 0 ;
            else*/
        skel_distance << p1_middle.length() ;
        static int total=0;
        static int border=0;
        static int exit=0;

        total++;
        /* if (point_is_border)
        {
            skel_distance << 0 ;
            border++;
        }
        else
            skel_distance << INFINITY ;*/
        exit += point_is_exit ;
        skel_point_is_exit << point_is_exit ;
        qDebug() << "border:" << border << " exit:" << exit << " total:" << total ;
    }
    return middle_idx;
}
void SkelWorker3::run()
{

    //Insert the polygons into a constrained triangulation
    ConstrainedDelaunayTriangulation & cdt = mainWindow->cdt;
    cdt.clear();

    cdt.insert(CGALPoint(0,0));
    cdt.insert(CGALPoint(0,mainWindow->openedQImage.height()-1));
    cdt.insert(CGALPoint(mainWindow->openedQImage.width()-1,mainWindow->openedQImage.height()-1));
    cdt.insert(CGALPoint(mainWindow->openedQImage.width()-1,0));

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
    cdt.mark_domains();


    middles_idxs.clear();
    skel_points.clear();
    skel_children.clear() ;
    skel_distance.clear() ;
    skel_point_is_exit.clear();

    mainWindow->point_cloud.handles.clear(); ;
    mainWindow->point_cloud.pts.clear(); ;
    mainWindow->treeIdx2Handle.clear(); ;


    for (Face_iterator it = mainWindow->cdt.finite_faces_begin() ; it!=mainWindow->cdt.finite_faces_end() ; ++it)
    {
        if (!it->info().in_domain())
        {
            Triangulation::Face_handle f0 = it->neighbor(0);
            Triangulation::Face_handle f1 = it->neighbor(1);
            Triangulation::Face_handle f2 = it->neighbor(2);

            bool f0_in_domain = f0->info().in_domain() ;
            bool f1_in_domain = f1->info().in_domain() ;
            bool f2_in_domain = f2->info().in_domain() ;

            Triangulation::Edge e0(it,0);
            Triangulation::Edge e1(it,1);
            Triangulation::Edge e2(it,2);

            Vertex_handle v0(it->vertex(0));
            Vertex_handle v1(it->vertex(1));
            Vertex_handle v2(it->vertex(2));

            int centroid_idx;




                QPointF p0(v0->point().x(),v0->point().y());
                QPointF p1(v1->point().x(),v1->point().y());
                QPointF p2(v2->point().x(),v2->point().y());

                double l0 = QLineF(p1,p2).length() ;
                double l1 = QLineF(p0,p2).length() ;
                double l2 = QLineF(p1,p0).length() ;

                double l = qMin(l0,qMin(l1,l2));

                if (l2==l)
                {
                    //l0>l1>l2
                    if (l2*3<l0+l1)
                    centroid_idx = create_cendroid(v0,v1,v2,v2) ;
                    else
                        centroid_idx = create_cendroid(v0,v1,v2) ;

                }
                else if (l1==l)
                {
                    if(l1*3<l0+l2)
                    centroid_idx = create_cendroid(v0,v1,v2,v1) ;
                    else
                        centroid_idx = create_cendroid(v0,v1,v2) ;

                }
                else
                {
                    if (l0*3<l1+l2)
                    centroid_idx = create_cendroid(v0,v1,v2,v0) ;
                    else
                        centroid_idx = create_cendroid(v0,v1,v2) ;

                }

            if (!f0_in_domain)
            {
                int middle0_idx = maibe_create_middle(v1,v2,cdt.is_infinite(f0));
                skel_children[centroid_idx] << middle0_idx ;
                skel_children[middle0_idx] << centroid_idx ;
            }
            if (!f1_in_domain)
            {
                int middle0_idx = maibe_create_middle(v0,v2,cdt.is_infinite(f1));
                skel_children[centroid_idx] << middle0_idx ;
                skel_children[middle0_idx] << centroid_idx ;
            }
            if (!f2_in_domain)
            {
                int middle0_idx = maibe_create_middle(v0,v1,cdt.is_infinite(f2));
                skel_children[centroid_idx] << middle0_idx ;
                skel_children[middle0_idx] << centroid_idx ;
            }
        }
    }
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
            // si c'est une impasse et que ce n'est pas une sortie.
            if ((v.count()==1)&&(!skel_point_is_exit[i]))
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

