/**
 * Simple representation of a vertex in a mesh with 
 * useful functions for edge collapse based on 
 * 
 * Progressive Mesh type Polygon Reduction Algorithm
 * by Stan Melax (c) 1998
 */
#ifndef VERTEX_HPP
#define VERTEX_HPP


#include <stdlib.h>
#include <vector>

#include "pandabase.h"
#include "luse.h"

#include "common.h"
#include "triangle.h"
class Triangle;

class Vertex {
  public:
    LVecBase3f position;  // location of point in euclidean space
    LVecBase3f normal;  // location of point in euclidean space
    int id;  // place of vertex in original Array
    std::vector<Vertex *> neighbor;  // adjacent vertices
    std::vector<Triangle *> face;  // adjacent triangles
    float objdist;  // cached cost of collapsing edge
    Vertex *collapse;  // candidate vertex for collapse
    Vertex(LVecBase3f v, int _id);
    virtual ~Vertex();
    void remove_if_non_neighbor(Vertex *n);
};
#endif