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
#include "pta_int.h"
#include "pnotify.h"
#include "geomVertexData.h"
#include "geomVertexWriter.h"
#include "geom.h"
#include "geomTriangles.h"
#include "geomNode.h"
#include "Python.h"

#include "common.h"
#include "triangle.h"
#include "vertex.h"

using namespace std;

template <class T> void print_pta(T _pta) {
  int pta_size = _pta.size();
  cout << "[";
  for (int i = 0; i < pta_size; i++) {
    if (i < 4 || pta_size < 12 || (pta_size >= 12 && pta_size - i < 5)) {
      if (i != 0) {
        cout << " ";
      }
      cout << "[" << _pta[i] << "]";
      if (i < pta_size - 1) {
        cout << endl;
      }
    }
    else if (i == 4 && pta_size >= 12) {
      cout << " ..." << endl;
      cout << " ..." << endl;
      cout << " ..." << endl;
    }
  }
  cout << "]" << endl;
}


class GeomStore {
  PUBLISHED:
    GeomStore();
    ~GeomStore();
    void print_vertices() {print_pta(_vertex_positions);};
    void print_colors() {print_pta(_colors);};  
    void print_triangles() {print_pta(_triangle_indices);};
    int add_vertex(LVecBase3f v);
    int add_vertex(LVecBase3f v, LVecBase4f c);
    int add_triangle(int v0, int v1, int v2);
    void subdivide_triangles(int s);
    void subdivide_triangles_distance(float d);
    void extend(GeomStore *other);
    void set_color(LVecBase4f c);
    void set_color(LVecBase4f c, PTA(int) filter);
    void normals_as_color();
    void to_unit_sphere();
    PT(GeomNode) get_p3d_geom_node(string name="UnnamedGeom");
    int operator + (float v);
    int operator + (LVecBase3f v);
    int operator - (float v);
    int operator - (LVecBase3f v);
    int operator * (float v);
    int operator * (LVecBase3f v);
    int operator / (float v);
    int operator / (LVecBase3f v);
    PTA_LVecBase3f _vertex_positions;
    PTA_LVecBase4f _colors;
    PTA_LVecBase3i _triangle_indices;
  private:
    Vertex* get_vertex(int id);
    int subdivide(float d=0.0f);
    vector<Vertex *> _vertices;
    vector<Triangle *> _triangles;
};

#endif