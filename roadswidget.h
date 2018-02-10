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
    void exploreEdge(int first, int second);
    void explorePoint(int i);
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
    QVector<QSet<int> > & roads_junctions ;
    QVector<QVector<int> > & roads_edges;
    QHash<int,int> & index_junction;
    QVector<QVector<unsigned int> > & skel_children;
    QVector<QPointF> & skel_vertices;
    QVector<float> & skel_distance ;
    QVector<unsigned int> &pre_roads_index_vbo;
    QVector<float> &pre_roads_vbo ;
    QVector<unsigned int> &pre_roads_index_vbo_start;
    QVector<unsigned int> &pre_roads_index_vbo_end;
    QVector<QVector<float> > &pre_junctions_vbos;
    QVector<int> &valid_junctions;
    QVector<int> &valid_roads ;


    QVector<QVector<QPointF> > & junctions_line_strings ;

    double radiusFactor;
    void exploreGraph();

    QVector<QPointF> cycles_centers;
    QVector<int> cycles_lengths ;
    QVector<QPolygonF> cycles_contours;

    // QWidget interface
    void addBranch(QVector<QLineF> &tree, const Arrival &arrival);
protected:
};

#endif // ROADSWIDGET_H
