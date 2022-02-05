#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "constraineddelaunaytriangulation.h"
#include "customtypes.h"
#include "doubleimage.h"
#include "historesult.h"
#include "intropage.h"
#include "kdtree2d.h"
#include "statsform.h"

#include <QHash>
#include <QLabel>
#include <QMainWindow>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLVertexArrayObject>
#include <QProgressBar>
#include <QPushButton>
#include <QStackedWidget>
#include <QUrl>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    QVector<QWidget*> history ;
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QImage initialImage;
    QImage croppedImage;
    QImage openedQImage;
    DoubleImage * regularizedImage ;
    DoubleImage * openedImage ;
    QProgressBar * progress ;

    QOpenGLVertexArrayObject contour_vao;
    QOpenGLBuffer quad_vbo;
    QOpenGLBuffer contour_vbo;
    QOpenGLBuffer *contour_index_vbo;
    QOpenGLShaderProgram *quad_program;
    QOpenGLShader *quad_vertex_shader;
    QOpenGLShader *quad_fragment_shader;
    QOpenGLShaderProgram *line_program;
    QOpenGLShader *line_vertex_shader;
    QOpenGLShader *line_fragment_shader;
    QOpenGLTexture *texture;
    QOpenGLBuffer skel_vbo;
    QOpenGLBuffer *skel_index_vbo;
    QOpenGLBuffer roads_vbo;
    QOpenGLBuffer *roads_index_vbo;
    QOpenGLBuffer faces_vbo;
    QOpenGLBuffer *faces_index_vbo;
    QOpenGLBuffer selection_vbo;
    QOpenGLBuffer circle_vbo;
    QOpenGLBuffer black_roads_vbo;

    ConstrainedDelaunayTriangulation cdt;

KDTree2D::PointCloud point_cloud;
    KDTree2D::my_kd_tree_2d_type *kdtree ;
    QVector<Vertex_handle> treeIdx2Handle;
    QMap<CGALTriangle,uint32_t> centersIdx;

    inline QVector<DoubleSidedEdge> & getDoubleSidedEdges(){return double_sided_edges;}
    inline QVector<QPointF> & getSkelVertices(){return skelVertices;}
    inline QVector<uint32_t> & getSkelIndices(){return skelIndices;}
    inline QVector<QVector<uint32_t> > & getSkelChildren(){return skelChildren;}
    inline QVector<unsigned char> & getSkelColors(){return skelColors;}
    inline QVector<float> & getSkelDistanceToBoundary(){return skelDistanceToBoundary;}
    inline QVector<bool> & getSkelPointIsExit() {return skel_point_is_exit;}
    inline QVector<QPointF> & getContourVertices(){return contourVertices;}
    inline QVector<uint32_t> & getContourIndices(){return contourIndices;}
    inline QVector<uint32_t> & getNextPointIndex(){return nextPointIndex;}
    inline QVector<uint32_t> & getPrevPointIndex(){return prevPointIndex;}
    inline DoubleImage * getOpenedImage(){return openedImage;}
    inline QVector<uint32_t> & getConnectedComponentsStarts(){return connectedComponentsStarts;}
    inline QVector<QVector<uint32_t> > & getRoadsEdges(){return roads_edges;}
    inline QVector<QSet<uint32_t> > & getRoadsJunctions(){return roads_junctions;}
    inline QMap<uint32_t,uint32_t> & getIndexJunction(){return index_junction;}
    inline QVector<Junction> & getJunctions(){return junctions;}
    inline QVector<double> & getEdgeCloseness(){return edgeCloseness;}
    void setActionsEnabled(bool enabled);
    QPushButton * back,*next;

    QVector<uint32_t> pre_roads_index_vbo;
    QVector<float> pre_roads_vbo ;
    QVector<uint32_t> pre_roads_index_vbo_start;
    QVector<uint32_t> pre_roads_index_vbo_end;
    QVector<uint32_t> pre_valid_roads_index_vbo_start;
    QVector<uint32_t> pre_valid_roads_index_vbo_end;
    QVector<uint32_t>  pre_valid_roads_index_vbo;

    QVector<QVector<float> > pre_junctions_vbos;
    QVector<QOpenGLBuffer*> junctions_contours_vbos;
    QVector<QOpenGLBuffer*> junctions_vbos;
    QVector<QOpenGLBuffer*> junctions_bg_vbos;

    QVector<QVector<uint32_t> > histoIntData;
    QVector<QVector<double> > histoDoubleData;

    QVector<int32_t> valid_roads;
    QVector<int32_t> valid_junctions;

    double valid_roads_total_length;

    QVector<QVector<QPointF> > roads_line_strings;
    QVector<QColor> roads_line_strings_colors;

    QVector<QVector<QPointF> > junctions_line_strings ;

    QVector<QVector<QPointF> > places_contours_line_strings;

    double ratio_black_pixels;

    double scale ;
    QPointF center;

    QString htmlLog;
    QStringList savedFilesPaths;

    void appendToSavedFiles(QString str);
    void trySaveImage(const QString & pre,const QImage & image);
    void trySaveDoubleImage(QString pre, DoubleImage *image);
    QSettings settings;
    void setPhase(int p);
    void batchProcess();
    void dynamicalAnalysis();

public slots :
    void nextPhase();
    void prevPhase();
    void receiveProgressIncrement(int inc);
    void openBrowser(QUrl link) ;
protected:
    void closeEvent(QCloseEvent *event);
private slots:

    void on_actionAide_2_triggered();

    void on_actionParam_tres_triggered();

private:

    static const int maxPhase = 9 ;

    Ui::MainWindow *ui;
    QLabel *title ;
    QLabel * statusLabel ;
    int phase ;
    QStackedWidget * stackedWidget ;

    QVector<QPointF> skelVertices ;
    QVector<uint32_t> skelIndices;
    QVector<unsigned char> skelColors;
    QVector<QVector<uint32_t> > skelChildren ;
    QVector<float> skelDistanceToBoundary ;
    QVector<bool> skel_point_is_exit ;

    QVector<DoubleSidedEdge> double_sided_edges ;
    QVector<DoubleSidedEdge> double_sided_edges_copy ;
    QVector<uint32_t> double_sided_edges_birth ;


    QVector<QPointF> contourVertices ;
    QVector<uint32_t> contourIndices;
QVector<uint32_t> nextPointIndex;
QVector<uint32_t> prevPointIndex;
QVector<uint32_t> connectedComponentsStarts ;

QVector<QSet<uint32_t> > roads_junctions;
QVector<Junction> junctions ;
QMap<uint32_t,uint32_t> index_junction ;
QVector<QVector<uint32_t> >  roads_edges;

QVector<double> edgeCloseness;

//QDockWidget *docWidget,*paramWidget;
void setPageTitle(QString str);

void restorePhase();
void savePhase();
void organizeSequenceFiles(QStringList dirs_paths);
bool isOnPicture(int e, double treshold);
} ;

#endif // MAINWINDOW_H
