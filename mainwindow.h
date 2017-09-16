#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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
#include <QStackedWidget>
#include <QToolButton>
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

    Triangulation cdt;
KDTree2D::PointCloud point_cloud;
    KDTree2D::my_kd_tree_2d_type *kdtree ;
    QVector<Vertex_handle> treeIdx2Handle;
    QMap<CGALTriangle,int> centersIdx;

    inline QVector<DoubleSidedEdge> & getDoubleSidedEdges(){return double_sided_edges;}
    inline QVector<QPointF> & getSkelVertices(){return skelVertices;}
    inline QVector<unsigned int> & getSkelIndices(){return skelIndices;}
    inline QVector<QVector<unsigned int> > & getSkelChildren(){return skelChildren;}
    inline QVector<unsigned char> & getSkelColors(){return skelColors;}
    inline QVector<float> & getSkelDistanceToBoundary(){return skelDistanceToBoundary;}
    inline QVector<QPointF> & getContourVertices(){return contourVertices;}
    inline QVector<unsigned int> & getContourIndices(){return contourIndices;}
    inline QVector<unsigned int> & getNextPointIndex(){return nextPointIndex;}
    inline QVector<unsigned int> & getPrevPointIndex(){return prevPointIndex;}
    inline DoubleImage * getOpenedImage(){return openedImage;}
    inline QVector<unsigned int> & getConnectedComponentsStarts(){return connectedComponentsStarts;}
    inline QVector<QVector<int> > & getRoadsEdges(){return roads_edges;}
    inline QVector<QSet<int> > & getRoadsJunctions(){return roads_junctions;}
    inline QHash<int,int> & getIndexJunction(){return index_junction;}
    inline QVector<Junction> & getJunctions(){return junctions;}
    void setActionsEnabled(bool enabled);
    QToolButton * back,*next;

    QVector<unsigned int> pre_roads_index_vbo;
    QVector<float> pre_roads_vbo ;
    QVector<unsigned int> pre_roads_index_vbo_start;
    QVector<unsigned int> pre_roads_index_vbo_end;
    QVector<unsigned int> pre_valid_roads_index_vbo_start;
    QVector<unsigned int> pre_valid_roads_index_vbo_end;
    QVector<unsigned int>  pre_valid_roads_index_vbo;

    QVector<QVector<float> > pre_junctions_vbos;
    QVector<QOpenGLBuffer*> junctions_contours_vbos;
    QVector<QOpenGLBuffer*> junctions_bg_vbos;

    QVector<QVector<int> > histoIntData;
    QVector<QVector<double> > histoDoubleData;

    QVector<int> valid_roads;
    double valid_roads_total_length;

    QVector<QVector<QPointF> > roads_line_strings;
    QVector<QColor> roads_line_strings_colors;
    QVector<QVector<QPointF> > places_contours_line_strings;

    double scale ;
    QPointF center;

    QStringList logStrings;

    void log(QString str);
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
    QVector<unsigned int> skelIndices;
    QVector<unsigned char> skelColors;
    QVector<QVector<unsigned int> > skelChildren ;
    QVector<float> skelDistanceToBoundary ;
    QVector<DoubleSidedEdge> double_sided_edges ;
    QVector<DoubleSidedEdge> double_sided_edges_copy ;
    QVector<int> double_sided_edges_birth ;


    QVector<QPointF> contourVertices ;
    QVector<unsigned int> contourIndices;
QVector<unsigned int> nextPointIndex;
QVector<unsigned int> prevPointIndex;
QVector<unsigned int> connectedComponentsStarts ;

QVector<QSet<int> > roads_junctions;
QVector<Junction> junctions ;
QHash<int,int> index_junction ;
QVector<QVector<int> >  roads_edges;

//QDockWidget *docWidget,*paramWidget;
void setPageTitle(QString str);

void restorePhase();
void savePhase();
void organizeSequenceFiles(QStringList dirs_paths);
bool isOnPicture(int e, double treshold);
} ;

#endif // MAINWINDOW_H
