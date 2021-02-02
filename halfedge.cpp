#include "halfedge.h"

#include <QMap>
#include <QSet>

HalfEdge::HalfEdge()
{

}

int HalfEdge::degreeOfVertex(uint32_t v)
{
    int degree = 0 ;
    for (int i = 0 ; i < edges.count() ;++i)
        if (edges[i].vertex == v)
            degree++;
    return degree;
}

HalfEdge *HalfEdge::suppressDeadEnds()
{
    QSet<uint32_t> edgesToSuppress;
    QSet<uint32_t> verticesToSuppress;
    for (int i = 0 ; i < vertices.count() ; ++i)
    {
        uint32_t degree = degreeOfVertex(i) ;
        if ((!vertices[i].exit)&&(degree==1))
        {
            verticesToSuppress.insert(i);
            for (int j = 0 ; j < edges.count() ; ++j)
            {
                if (edges[j].vertex==i)
                {
                    edgesToSuppress.insert(j);
                    edgesToSuppress.insert(edges[j].opposite);
                    break;
                }
            }

        }
        else
            if (degree==0)
            {
                verticesToSuppress << i ;
            }
    }
    HalfEdge * he = new HalfEdge();
    he->vertices = vertices;
    he->edges = edges ;
    he->faces = faces  ;
    he->suppressEdges(edgesToSuppress);
    he->suppressVertices(verticesToSuppress);
    return he;
}

HalfEdge *HalfEdge::suppressRoadsOfTopoLengthN(uint32_t N,bool & something_happend)
{
    QMap<uint32_t,uint32_t> roadIndexNumber;
    for(int i = 0 ; i < edges.count() ; ++i)
    {
        uint32_t index = edges[i].road ;
        if (roadIndexNumber.contains(index))
        {
            roadIndexNumber[index]=roadIndexNumber[index]+1;
        }
        else
        {
            roadIndexNumber[index]=1;
        }
    }
    QSet<uint32_t> roadIndexToSuppress;
    QList<uint32_t> keys = roadIndexNumber.keys();
    QVector<Face> newFaces;
    QVector<QSet<uint32_t> > facesToFuse;
    for (int i = 0 ; i < keys.count() ; ++i)
    {
        if (roadIndexNumber[keys[i]]==2*N)
        {
            roadIndexToSuppress << keys[i];
            something_happend = true ;
        }
    }
    QSet<uint32_t> edgesToSuppress;
    QSet<uint32_t> fusedFaces;
    for (int i = 0 ; i < edges.count() ; ++i)
    {
        if (roadIndexToSuppress.contains(edges[i].road))
        {
            edgesToSuppress << i ;
            bool found = false;
            for (int j = 0 ; j < facesToFuse.count();++j)
            {
                if ((facesToFuse[j].contains(edges[i].face))||(facesToFuse[j].contains(edges[edges[i].opposite].face)))
                {
                    if (edges[i].face < faces.count())
                    facesToFuse[j] << edges[i].face ;
                    if (edges[edges[i].opposite].face < faces.count())
                    facesToFuse[j] << edges[edges[i].opposite].face ;

                    found = true;
                    break;
                }
            }
            if (!found)
            {
                QSet<uint32_t> set;
                if (edges[i].face < faces.count())
                set << edges[i].face ;
                if (edges[edges[i].opposite].face < faces.count())
                set  << edges[edges[i].opposite].face ;
                facesToFuse << set ;
            }
            fusedFaces << edges[i].face ;
            fusedFaces << edges[edges[i].opposite].face ;

        }
    }
    HalfEdge * he = new HalfEdge();
    he->edges = edges ;
    he->suppressEdges(edgesToSuppress);

    he->vertices = vertices ;

    QSet<uint32_t> faceTreated;
    for (int i = 0 ; i < faces.count() ; ++i)
    {
        if (faceTreated.contains(i)) continue;
        if (fusedFaces.contains(i))
        {
            int j = 0;
            while (j<facesToFuse.count())
            {
                if (facesToFuse[j].contains(i))
                    break;
                j++;
            }
            Face f;
            f.childrens = facesToFuse[j];
            QList<uint32_t> list = f.childrens.values();
            faceTreated << list[0];
            QPolygonF p = faces[list[0]].contour;
            double s = faces[list[0]].surface;
            faces[list[0]].parent = he->faces.count() ;
            f.stream_order = faces[list[0]].stream_order;
                uint32_t max = f.stream_order ;
            for (int k =1;k<list.count();++k)
            {
                faceTreated << list[k];
                p = p.united(faces[list[k]].contour);
                s += faces[list[k]].surface;
                faces[list[k]].parent = he->faces.count() ;
                if (faces[list[k]].stream_order>max)
                {
                    max = faces[list[k]].stream_order ;
                    f.stream_order = max ;
                }
                else if (faces[list[k]].stream_order==max)
                {
                    f.stream_order = max + 1;
                }

            }
            f.surface = s;
            f.contour = p ;
            he->faces << f ;
        }
        else {
            Face f ;
            f.childrens << i ;
            f.surface = faces[i].surface;
            f.contour = faces[i].contour ;
            faces[i].parent = he->faces.count();
            f.stream_order = faces[i].stream_order ;
            he->faces << f ;
        }
    }
    for (int i = 0 ; i < he->edges.count();++i)
    {
        Edge & e = he->edges[i];
        if (e.face<faces.count())
            e.face = faces[e.face].parent ;
    }
    return he;
}

void HalfEdge::suppressVertices(const QSet<uint32_t> &verticesToSuppress)
{
    QVector<Vertex> newVertices;
    QVector<uint32_t> newIndices;
    for (int i = 0 ; i < vertices.count() ; ++i)
    {
        if (verticesToSuppress.contains(i))
        {
            newIndices << -1 ;
        }
        else
        {
            newIndices << newVertices.count() ;
            newVertices << vertices[i];
        }
    }
    vertices = newVertices ;
    for (int i = 0 ; i < edges.count() ; ++i)
    {
        Edge & e = edges[i];
        e.vertex = newIndices[e.vertex];
        if (e.vertex>=vertices.count())
        {

        }
    }
}

void HalfEdge::suppressEdges(const QSet<uint32_t> &edgesToSuppress)
{
    QVector<uint32_t> newIndices;
    QVector<Edge> newEdges;
    for (int i = 0 ; i < edges.count() ; ++i)
    {
        if (edgesToSuppress.contains(i))
        {
            newIndices << -1 ;
        }
        else
        {
            newIndices << newEdges.count() ;
            newEdges << edges[i];
        }
    }
    for (int i = 0 ; i < newEdges.count() ; ++i)
    {
        Edge & e = newEdges[i];
        while (newIndices[e.next]>=newEdges.count())
        {
            e.next = edges[edges[e.next].opposite].next ;
        }
        e.next = newIndices[e.next];
        e.opposite = newIndices[e.opposite];
    }
//    for (int i = 0 ; i < vertices.count() ; ++i)
//    {
//        Vertex & v = vertices[i];
//        while (newIndices[v.edge]>=newEdges.count())
//        {
//            v.edge = edges[edges[v.edge].opposite].next ;
//        }
//        v.edge = newIndices[v.edge];
//    }
    edges = newEdges;
}

HalfEdge *HalfEdge::suppressVerticesOfDegree2()
{
    HalfEdge * he = new HalfEdge;
    he->edges = edges;
    he->vertices = vertices;


     bool something_happened = true;
    while (something_happened)
    {
        something_happened = false ;
        QVector<QVector<uint32_t> > excidentEdges(he->vertices.count(),QVector<uint32_t>());
         for (int i = 0 ; i < he->edges.count() ; ++i)
         {
             excidentEdges[he->edges[i].vertex]<<i ;

         }
         for (int i = 0 ; i < he->vertices.count() ; ++i)
         {
             if (excidentEdges[i].count()==2) {
                 QSet<uint32_t> verticesToSuppress;
                 QSet<uint32_t> edgesToSuppress;
                 verticesToSuppress << i ;
                 Edge & e1 = he->edges[excidentEdges[i][0]];
                 Edge & e2 = he->edges[excidentEdges[i][1]];
                 e1.vertex = he->edges[e2.opposite].vertex;
                 e2.vertex = he->edges[e1.opposite].vertex;
                 if ((e1.vertex>=vertices.count())||(e2.vertex>=vertices.count()))
                 {

                 }
                 edgesToSuppress << e1.opposite ;
                 edgesToSuppress << e2.opposite ;
                 for (int j = 0 ; j < he->edges.count();++j)
                 {
                     if (he->edges[j].next == e2.opposite)
                         he->edges[j].next = excidentEdges[i][0] ;
                     if (he->edges[j].next== e1.opposite)
                         he->edges[j].next = excidentEdges[i][1] ;
                 }
                 e1.opposite = excidentEdges[i][1];
                 e2.opposite = excidentEdges[i][0];
                 e1.road=e2.road;
                 he->suppressEdges(edgesToSuppress);
                 he->suppressVertices(verticesToSuppress);
                something_happened = true ;
                break;
             }
         }
    }


    he->verifyIntegrity();
    he->faces = faces ;
    return he;
}

void HalfEdge::verifyIntegrity()
{
    for (int i = 0 ; i < edges.count();++i)
    {
        if (edges[i].next>=edges.count())
        {

        }
        if (edges[i].opposite>=edges.count())
        {

        }
        if (edges[i].vertex>=vertices.count())
        {

        }
    }

}
