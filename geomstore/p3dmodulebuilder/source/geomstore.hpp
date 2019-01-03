/**
 * C++ Python module to store an arbitrary 3D mesh and 
 * enable various functions:
 * 
 * -> edge collapse for dynamic LOD
 * -> smooth & flat vertex normal computation 
 * -> mesh edge stitching
 * -> triangle subdivision
 * -> etc.
 * 
 */

#ifndef GEOMSTORE_HPP
#define GEOMSTORE_HPP

#include "pandabase.h"
#include "luse.h"

#include "common.hpp"
#include "triangle.hpp"
#include "vertex.hpp"

int main();

class GeomStore {
  public:
    GeomStore();
    virtual ~GeomStore();
  PUBLISHED:
    int add_vertex(LVecBase3f v, LVecBase4f c);
    int add_triangle(int v0, int v1, int v2);
    void subdivide_triangles(int s = 1);
    void subdivide_triangles_distance(float d = 2.0f);
    void extend(GeomStore *other);
};

#endif