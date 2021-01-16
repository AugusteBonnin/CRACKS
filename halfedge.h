#include <QPointF>
#include <QPolygonF>

#pragma once

typedef struct {
uint32_t junction;
uint32_t edge;
bool exit ;
QPointF center;
}Vertex;

typedef struct {
    double surface;
    uint32_t edge;
    QPolygonF contour;
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

};

