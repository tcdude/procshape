/**
 * C++ Python extension module to store an arbitrary 3D mesh and 
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

#include "config_module.h"
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
      cout << " ..." << endl << " ..." << endl << " ..." << endl;
    }
  }
  cout << "]" << endl;
}


class GeomStore {
  PUBLISHED:
    GeomStore(int v_reserve = (int)1e6, int t_reserve = (int)1e6);
    ~GeomStore();
    void print_vertices() {print_pta(_vertex_positions);};
    void print_colors() {print_pta(_colors);};  
    void print_triangles() {print_pta(get_triangle_indices());};
    int add_vertex(LVecBase3f point);
    int add_vertex(LVecBase3f point, LVecBase4f color);
    int add_triangle(int v0, int v1, int v2);
    int add_quad(int v0, int v1, int v2, int v3);
    bool mirror(int axis);
    bool flip_faces();
    void subdivide_triangles(int subdivisions = 1);
    void subdivide_triangles_distance(float target_distance = 2.0f);
    void subdivide_triangles_spheroid(float target_distance = 2.0f, 
      LVecBase3f bb = LVecBase3f(1.0f));
    void extend(GeomStore *other);
    void set_color(LVecBase4f color);
    void set_color(LVecBase4f color, PTA(int) selection);
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
    PTA_LVecBase3i get_triangle_indices();
    PTA_LVecBase3f _vertex_positions;
    PTA_LVecBase4f _colors;
  private:
    Vertex* get_vertex(int id);
    int subdivide(Triangle* t, bool s = false, LVecBase3f bb = LVecBase3f(1.0f));
    vector<Vertex *> _vertices;
    vector<Triangle *> _triangles;
    void mult_vec(LVecBase3f v);
    int _v_reserve;
    int _t_reserve;
};

#endif