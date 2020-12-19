#include <QPointF>
#include <QPolygonF>

#pragma once

typedef struct {
QPointF point;
int32_t edge;
}Vertex;

typedef struct {
    double surface;
    int32_t edge;
}Face;

typedef struct
{
    int32_t opposite;
    int32_t next;
    int32_t vertex;
    int32_t face;
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

