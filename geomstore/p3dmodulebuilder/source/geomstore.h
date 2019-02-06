/**
 * Representation of arbitrary geometry, stored in OpenMesh Kernel and
 * accessible through memoryview in Python. Provides subdivision, mesh
 * decimation smoothing
 * 
 * @file geomstore.cpp
 * @author Tiziano Bettio
 * @date 2019-01-10
 */


#ifndef GEOMSTORE_H
#define GEOMSTORE_H

#include <string>

#include "Python.h"
#include "pandabase.h"
#include "pta_LVecBase3.h"
#include "pta_LVecBase4.h"
#include "pta_int.h"

#include "common.h"

class Mesh;
class GeomNode;

typedef std::string string;

class GeomStore {
  PUBLISHED:
    GeomStore();
    ~GeomStore();
    void clear();
    void print_vertices() { print_pta(_points); };
    void print_colors() { print_pta(_colors); };  
    void print_triangles() { print_pta(get_triangle_indices()); };
    void set_num_rows(int rows, int triangles = 0);
    int add_vertex(LVecBase3f point);
    int add_vertex(LVecBase3f point, LVecBase4f color);
    int add_triangle(int v0, int v1, int v2);
    int add_quad(int v0, int v1, int v2, int v3);
    bool mirror(int axis);
    bool flip_faces();
    bool rotate(LQuaternionf q);
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
    PTA_LVecBase3i get_triangle_indices();
    LVecBase3f get_vertex(int vid);
    int operator + (float v);
    int operator + (LVecBase3f v);
    int operator - (float v);
    int operator - (LVecBase3f v);
    int operator * (float v);
    int operator * (LVecBase3f v);
    int operator / (float v);
    int operator / (LVecBase3f v);
    /*PTA_LVecBase3f get_pta();
    void set_pta(PTA_LVecBase3f);*/
  public:
    PTA_LVecBase3f _points;
    PTA_LVecBase4f _colors;
    PTA_LVecBase3i _triangles;
#ifndef CPPPARSER
    Mesh* _my_mesh;
#endif
};

#endif