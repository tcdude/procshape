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

#include "common.h"
#include "triangle.h"
#include "vertex.h"

class GeomStore {
  PUBLISHED:
    GeomStore();
    virtual ~GeomStore();
    int add_vertex(LVecBase3f v, LVecBase4f c);
    int add_triangle(int v0, int v1, int v2);
    void subdivide_triangles(int s);
    void subdivide_triangles_distance(float d);
    void extend(GeomStore *other);
};

#endif