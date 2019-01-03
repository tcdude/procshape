/**
 * Simple representation of a triangle in a mesh with 
 * useful functions for edge collapse based on 
 * 
 * Progressive Mesh type Polygon Reduction Algorithm
 * by Stan Melax (c) 1998
 */

#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP


#include <vector>

#include "pandabase.h"
#include "luse.h"

#include "common.h"
#include "vertex.h"
class Vertex;

class Triangle {
  public:
    Triangle(Vertex *v0, Vertex *v1, Vertex *v2);
    virtual ~Triangle();
    Vertex *vertex[3];  // the 3 points that make this tri
    LVecBase3f normal;  // unit vector orthogonal to this face
    void compute_normal();
    void replace_vertex(Vertex *v_old, Vertex *v_new);
    int has_vertex(Vertex *v);
};

#endif