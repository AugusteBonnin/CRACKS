#include <QPointF>
#include <QPolygonF>
#include <QSet>

#pragma once

typedef struct {
uint32_t junction;
//uint32_t edge;
bool exit ;
QPointF center;
}Vertex;

typedef struct {
    double surface;
    QPolygonF contour;
    uint32_t parent;
    QSet<uint32_t> childrens;
    uint32_t stream_order;
}Face;

typedef struct
{
    uint32_t opposite;
    uint32_t next;
    uint32_t vertex;
    uint32_t face;
    uint32_t road;
    QPolygonF str;
}Edge;




  class HalfEdge
{
    public:
    HalfEdge();
QVector<Vertex> vertices;
QVector<Edge> edges;
QVector<Face> faces;
int degreeOfVertex(uint32_t i);
HalfEdge * suppressDeadEnds();
HalfEdge * suppressRoadsOfTopoLengthN(uint32_t N, bool &something_happend);
void suppressVertices(const QSet<uint32_t> & verticesToSuppress);
void suppressEdges(const QSet<uint32_t> & edgesToSuppress);
HalfEdge * suppressVerticesOfDegree2();
void verifyIntegrity();
  };

