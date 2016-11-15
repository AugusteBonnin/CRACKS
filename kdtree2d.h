#ifndef KDTREE2D_H
#define KDTREE2D_H

#include "nanoflann.hpp"

#include <customtypes.h>


using namespace std;
using namespace nanoflann;

class KDTree2D
{public :
    struct PointCloud
    {
        struct Point
        {
            float x,y;
        };

        vector<Point> pts;
        vector<Vertex_handle> handles;

        inline size_t kdtree_get_point_count() const {return pts.size();}
        inline float kdtree_distance(const float * p1,const size_t idx_p2, size_t)const
        {
            const float d0 = p1[0]-pts[idx_p2].x;
            const float d1 = p1[1]-pts[idx_p2].y;
            return d0*d0+d1*d1;
            }

        inline float kdtree_get_pt(const size_t idx,int dim) const
        {
            if (dim == 0)
                return pts[idx].x ;
            else
                return pts[idx].y ;
        }

        template < class BBOX>
        bool kdtree_get_bbox(BBOX&) const { return false;}

    };
    typedef KDTreeSingleIndexAdaptor<
    L2_Simple_Adaptor<float,PointCloud>,
    PointCloud,
    2
    > my_kd_tree_2d_type ;

public:
    KDTree2D();
};


#endif // KDTREE2D_H
