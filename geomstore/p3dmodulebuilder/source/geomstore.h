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
 * @file geomstore.h
 * @author Tiziano Bettio
 * @date 2018-12-29
 */

#ifndef GEOMSTORE_HPP
#define GEOMSTORE_HPP

#include <iostream>
#include <vector>

#include "pandabase.h"
#include "luse.h"
#include "pta_LVecBase3.h"
#include "pta_LVecBase4.h"
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
    ~GeomStore();
    void print_vertices();
    void print_colors();
    void print_triangles();
    int add_vertex(LVecBase3f v, LVecBase4f c);
    int add_triangle(int v0, int v1, int v2);
    void subdivide_triangles(int s);
    void subdivide_triangles_distance(float d);
    void extend(GeomStore *other);
    int operator + (float v);
    int operator + (LVecBase3f v);
    int operator - (float v);
    int operator - (LVecBase3f v);
    int operator * (float v);
    int operator / (float v);
    PTA_LVecBase3f _vertex_positions;
    PTA_LVecBase4f _colors;
    PTA_LVecBase3i _triangle_indices;
  private:
    Vertex* get_vertex(int id);
    int subdivide(float d=0.0f);
    std::vector<Vertex *> _vertices;
    std::vector<Triangle *> _triangles;
    void print_pta(PTA_LVecBase3f _pta);
    void print_pta(PTA_LVecBase3i _pta);
    void print_pta(PTA_LVecBase4f _pta);
};

#endif