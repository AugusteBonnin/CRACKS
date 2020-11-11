#ifndef ROADSWIDGET_H
#define ROADSWIDGET_H

#include "customtypes.h"
#include "page.h"
#include "scrollableopenglwidget.h"

#include <QObject>

class RoadsWidget : public ScrollableOpenGLWidget
{
    Q_OBJECT
public:
    RoadsWidget(Page *parent);

    // QOpenGLWidget interface
public slots:
    void buildRoads(double radiusFactor, double threshold_on_B);
protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();
private:
    QRgb randColor();
    void exploreEdge(uint32_t first, uint32_t second);
    void explorePoint(uint32_t i);
    void computeJunctionsHulls();
    void computeJunctionsLineStrings();
    void computeJunctionsMeanRadius();
    void computeCloseness();
    void computeOtherIndices();
    void computeRoadsLineStrings();
    void computeDegreeOfJunctions();
    void computeRoadsJunctionsAndEdges();
    void computeEdgesRoadIndex();
    void matchJunctionsEdges(float threshold_on_B);
    QVector<Junction> & junctions ;
    QVector<DoubleSidedEdge> & double_sided_edges;
    QVector<QSet<uint32_t> > & roads_junctions ;
    QVector<QVector<uint32_t> > & roads_edges;
    QHash<uint32_t,uint32_t> & index_junction;
    QVector<QVector<uint32_t> > & skel_children;
    QVector<QPointF> & skel_vertices;
    QVector<float> & skel_distance ;
    QVector<uint32_t> &pre_roads_index_vbo;
    QVector<float> &pre_roads_vbo ;
    QVector<uint32_t> &pre_roads_index_vbo_start;
    QVector<uint32_t> &pre_roads_index_vbo_end;
    QVector<QVector<float> > &pre_junctions_vbos;
    QVector<int32_t> &valid_junctions;
    QVector<int32_t> &valid_roads ;


    QVector<QVector<QPointF> > & junctions_line_strings ;

    double radiusFactor;
    void exploreGraph();

    QVector<QPointF> cycles_centers;
    QVector<uint32_t> cycles_lengths ;
    QVector<QPolygonF> cycles_contours;

    // QWidget interface
    void addBranch(QVector<QLineF> &tree, const Arrival &arrival);
protected:
};

#endif // ROADSWIDGET_H
