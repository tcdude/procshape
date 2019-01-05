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

#include <vector>

#include "pandabase.h"
#include "luse.h"
#include "Python.h"

#include "common.h"
#include "triangle.h"
#include "vertex.h"

struct InternalBufferData {
  std::vector<float> _handle;
  Py_ssize_t _num_rows;
  Py_ssize_t _stride;
  std::string _format;
};

class GeomStore {
  PUBLISHED:
    GeomStore();
    virtual ~GeomStore();
    int add_vertex(LVecBase3f v, LVecBase4f c);
    int add_triangle(int v0, int v1, int v2);
    void subdivide_triangles(int s);
    void subdivide_triangles_distance(float d);
    void extend(GeomStore *other);
    int __getbuffer__(PyObject *self, Py_buffer *view, int flags);
    void __releasebuffer__(PyObject *self, Py_buffer *view) const;
    std::vector<float> my_vec;
    int operator + (float v);
    int operator + (LVecBase3f v);
    int operator - (float v);
    int operator - (LVecBase3f v);
    int operator * (float v);
    int operator / (float v);
  private:
    std::vector<Vertex *> _vertices;
    std::vector<Triangle *> _triangles;
};

#endif