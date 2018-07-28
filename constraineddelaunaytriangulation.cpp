#include "constraineddelaunaytriangulation.h"

ConstrainedDelaunayTriangulation::ConstrainedDelaunayTriangulation()
{

}

void
ConstrainedDelaunayTriangulation::mark_domains(Face_handle start,
             int index,
             std::list<Edge>& border )
{
    if(start->info().nesting_level != -1){
        return;
    }
    std::list<Face_handle> queue;
    queue.push_back(start);
    while(! queue.empty()){
       Face_handle fh = queue.front();
        queue.pop_front();
        if(fh->info().nesting_level == -1){
            fh->info().nesting_level = index;
            for(int i = 0; i < 3; i++){
                Edge e(fh,i);
                Face_handle n = fh->neighbor(i);
                if(n->info().nesting_level == -1){
                    if (is_constrained(e)) border.push_back(e);
                    else queue.push_back(n);
                }
            }
        }
    }
}
//explore set of facets connected with non constrained edges,
//and attribute to each such set a nesting level.
//We start from facets incident to the infinite vertex, with a nesting
//level of 0. Then we recursively consider the non-explored facets incident
//to constrained edges bounding the former set and increase the nesting level by 1.
//Facets in the domain are those with an odd nesting level.
void
ConstrainedDelaunayTriangulation::mark_domains()
{
    for(All_faces_iterator it = all_faces_begin(); it != all_faces_end(); ++it){
        it->info().nesting_level = -1;
    }
    std::list<Edge> border;
    mark_domains(infinite_face(), 0, border);
    while(! border.empty()){
       Edge e = border.front();
        border.pop_front();
        Face_handle n = e.first->neighbor(e.second);
        if(n->info().nesting_level == -1){
            mark_domains(n, e.first->info().nesting_level+1, border);
        }
    }
}
