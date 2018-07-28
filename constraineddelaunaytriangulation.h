#ifndef CONSTRAINEDDELAUNAYTRIANGULATION_H
#define CONSTRAINEDDELAUNAYTRIANGULATION_H

#include "customtypes.h"

class ConstrainedDelaunayTriangulation : public Triangulation
{
public:
    ConstrainedDelaunayTriangulation();
    void mark_domains();
private:
    void mark_domains(Face_handle start, int index, std::list<Edge> &border);
};

#endif // CONSTRAINEDDELAUNAYTRIANGULATION_H
