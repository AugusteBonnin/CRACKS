#ifndef CUSTOMTYPES
#define CUSTOMTYPES

#include<math.h>
#include <QHash>
#include <QPointF>
#include <QRgb>
#include <QSet>
#include <QVector>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Constrained_Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>
#include <CGAL/Polygon_2.h>
#include <iostream>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Constrained_Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_hierarchy_2.h>
#include <CGAL/Constrained_triangulation_plus_2.h>
#include <QMap>
#include <QLineF>
#include <QPointF>
#include <QVector3D>

struct FaceInfo2
{
    FaceInfo2(){}
    int nesting_level;
    bool in_domain(){
        return nesting_level%2 == 1;
    }
};

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_2<K>             Vbb;
typedef CGAL::Triangulation_hierarchy_vertex_base_2<Vbb> Vb;
typedef CGAL::Triangulation_face_base_with_info_2<FaceInfo2,K>    Fbb;
typedef CGAL::Constrained_triangulation_face_base_2<K,Fbb>        Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>      TDS;
typedef CGAL::No_intersection_tag                       Itag;
typedef CGAL::Constrained_Delaunay_triangulation_2<K,TDS,Itag> CDT;
typedef CGAL::Triangulation_hierarchy_2<CDT>             CDTH;
typedef CGAL::Constrained_triangulation_plus_2<CDTH>     Triangulation;
typedef Triangulation::Point                             CGALPoint;
typedef CGAL::Polygon_2<K>                                        Polygon_2;
typedef Triangulation::Edge_iterator  Edge_iterator;
typedef Triangulation::Finite_vertices_iterator  Vertex_iterator;
typedef Triangulation::Vertex_circulator  Vertex_circulator;
typedef Triangulation::Finite_faces_iterator Face_iterator ;
typedef Triangulation::Vertex_handle Vertex_handle;



typedef struct CGALTriangle {
    std::vector<Vertex_handle> vertices;
    CGALTriangle(Vertex_handle v1,Vertex_handle v2,Vertex_handle v3)
    {
        vertices.push_back(v1);
        vertices.push_back(v2);
        vertices.push_back(v3);
        std::sort(vertices.begin(),vertices.end());
    }

    CGALTriangle() {}

    CGALTriangle(const CGALTriangle & other) {vertices=other.vertices;}

    inline bool operator<(const CGALTriangle &other)  const
    {
        if (other.vertices[0]!=vertices[0])
            return (vertices[0] < other.vertices[0]);
        else
            if (other.vertices[1]!=vertices[1])
                return (vertices[1] < other.vertices[1]);

        return (vertices[2] < other.vertices[2]);
    }

    QPointF center()
    {
        QPointF inter;

        QPointF A(vertices[0]->point().x(),vertices[0]->point().y());
        QPointF B(vertices[1]->point().x(),vertices[1]->point().y());
        QPointF C(vertices[2]->point().x(),vertices[2]->point().y());

        QLineF AB(A,B) ;
        QLineF medAB = AB.normalVector() ;
        medAB.translate(AB.pointAt(.5)-A);

        QLineF AC(A,C) ;
        QLineF medAC = AC.normalVector() ;
        medAC.translate(AC.pointAt(.5)-A);

        medAB.intersect(medAC,&inter);

        return inter ;
    }

} CGALTriangle ;



typedef struct DoubleSidedEdge {
    QRgb color;
    uint32_t road_index ;
    uint32_t first_junction,second_junction;
    uint32_t first_index,second_index;
    QVector<uint32_t> str,truncated_str;
    double mean_distance;
    double length;
    double first_width,second_width ;
    } DoubleSidedEdge ;

typedef struct Arrival{
    double angle;
    uint32_t edge;
    QPointF point;
    bool str_inverted ;
    Arrival(){}
    Arrival(const Arrival & other)
    {
        angle = other.angle ;
        edge = other.edge ;
        point = other.point ;
        str_inverted = other.str_inverted ;
    }
} Arrival;

typedef struct Junction {
    QRgb color;
    float mean_radius ;
    QVector<uint32_t> centers_indices;
QVector<Arrival> arrivals;
QHash<uint32_t,uint32_t> equivalent;
QSet<uint32_t> roads;
Junction (const Junction & right)
    {
    color = right.color ;
    centers_indices = right.centers_indices ;
    arrivals = right.arrivals ;
}
Junction()
{
    }
} Junction ;

typedef struct EdgePair {
    float B ;
    int i,j;
    QPointF point_i,point_j;
    bool operator < (const EdgePair & right) const {return B <right.B ; }
} EdgePair ;


#endif // CUSTOMTYPES

