/**
 * @file geomstore.cpp
 * @author Tiziano Bettio
 * @date 2019-01-10
 */

#include "geomstore.h"

#include <iostream>
#include <algorithm>

#include "luse.h"
#include "pnotify.h"
#include "geomVertexData.h"
#include "geomVertexWriter.h"
#include "geom.h"
#include "geomTriangles.h"
#include "geomNode.h"

#include "config_module.h"
#include "mesh.h"


using namespace std;


/**
 * Constructor
 */
GeomStore::
GeomStore() {
  set_num_rows(8);  // at least reserve a little...
  _my_mesh = new Mesh();
}


/**
 * Destructor 
 */
GeomStore::
~GeomStore() {
  delete _my_mesh;
}


/**
 * Clear GeomStore
 */
void GeomStore::
clear() {
  int pc = _points.capacity();
  int tc = _triangles.capacity();
  _points.clear();
  _points.reserve(pc);
  _colors.clear();
  _colors.reserve(pc);
  _triangles.clear();
  _triangles.reserve(tc);
}

/**
 * Reserve memory for PTAs
 */
void GeomStore::
set_num_rows(int rows, int triangles) {
  _points.reserve(rows);
  _colors.reserve(rows);
  if (triangles <= 0) {
    _triangles.reserve(rows * 2);
  }
  else {
    _triangles.reserve(triangles);
  }
}


/**
 * Adds a vertex to the GeomStore and returns its corresponding id.
 */
int GeomStore::
add_vertex(LVecBase3f point) {
  return add_vertex(point, LVecBase4f(1.0f));
}


/**
 * Adds a vertex to the GeomStore and returns its corresponding id.
 */
int GeomStore::
add_vertex(LVecBase3f point, LVecBase4f color) {
  pair<bool, int> result = find_in_pta(_points, point);
  if (result.first && (LVecBase4f)_colors[result.second] == color) {
    return result.second;
  } 
  int id = _points.size();
  _points.push_back(point);
  _colors.push_back(color);
  geomstore_cat.warning() << "Added Point with ID " << id << endl;
  return id;
}


/**
 * Add a triangle to the GeomStore.
 * Returns the triangle id on success, -1 on error.
 */
int GeomStore:: 
add_triangle(int v0, int v1, int v2) {
  if ((int)_triangles.size() >= (int)_triangles.capacity()) {
    geomstore_cat->error() << "Not enough capacity reserved!! Capacity is: " 
      << (int)_triangles.capacity() << endl;
  }
  nassertr((int)_triangles.size() < (int)_triangles.capacity(), -1);
  nassertr(v0 >= 0 && v1 >= 0 && v2 >= 0, -1);
  if (v0 >= _points.size() || v1 >= _points.size() || v2 >= _points.size()) {
    geomstore_cat->error() << "Vertex index/indices out of bounds! Got (" 
      << v0 << "/" << v1 << "/" << v2 << "). Current size is " 
      << _points.size() << endl;
  }
  nassertr(v0 < _points.size() && v1 < _points.size() && v2 < _points.size(), -1);
  int id = _triangles.size();
  _triangles.push_back(LVecBase3i(v0, v1, v2));
  return id;
}


/**
 * Add a quad to the GeomStore. Creates two triangles with consecutive ids.
 * Returns the first triangle id on success, -1 on error.
 */
int GeomStore:: 
add_quad(int v0, int v1, int v2, int v3) {
  int t0 = add_triangle(v0, v1, v2);
  int t1 = add_triangle(v2, v3, v0);
  nassertr(t0 > -1 && t1 > -1, -1);
  return t0;
}


/**
 * Mirror the contents of GeomStore on the specified `axis`by multiplying the 
 * values by -1. This only works, if the GeomStore does not contain geometry
 * with both positive and negative values of the indicated axis. Use with 
 * caution!
 */
bool GeomStore::
mirror(int axis) {
  nassertr(axis >= 0 && axis < 3, false);
  GeomStore *g = new GeomStore();
  g->set_num_rows(_points.size());
  g->extend(this);
  LVecBase3f m = 1.0f;
  m[axis] = -1.0f;
  g->operator*(m);
  g->flip_faces();
  extend(g);
  delete g;
  return true;
}


/**
 * Flips the winding of all faces.
 */
bool GeomStore::
flip_faces() {
  for (int i = 0; i < _triangles.size(); i++) {
    swap(_triangles[i][0], _triangles[i][2]);
  }
  return true;
}

/**
 * Rotates the GeomStore using the provided Quaternion `q`.
 */
bool GeomStore::
rotate(LQuaternionf q) {
  for (LVecBase3f& it : _points) {
    it = q.xform(it);
  }
  return true;
}

/**
 * Subdivide triangles s times. This is useful when the triangles already are
 * approximately the same size over the entire GeomStore.
 */
void GeomStore::
subdivide_triangles(int subdivisions) {
  _my_mesh->subdivision(this, subdivisions);
}


/**
 * Subdivides triangles until every hypothenuse is <= d (length in model view).
 * Use this when the triangles in GeomStore are of mixed size. Less efficient 
 * than subdivide_triangles(int s).
 */
void GeomStore::
subdivide_triangles_distance(float target_distance) {
  _my_mesh->subdivision_length(this, (double) target_distance);
}


/**
 * Subdivides triangles until every hypothenuse is <= d (length in model view)
 * while also performing the following steps after each subdivision:
 *  1. normalize the new vertex to unit length
 *  2. multiply the vector by bb (Bounding Box)
 */
void GeomStore::
subdivide_triangles_spheroid(float target_distance, LVecBase3f bb) {
}


/**
 * Extend this GeomStore with all Vertices and Triangles from other while
 * leaving other intact (New objects are being generated!).
 */
void GeomStore::
extend(GeomStore *other) {
  int new_size = _points.size() + other->_points.size();
  if (new_size > _points.capacity()) {
    set_num_rows(new_size);
  }
  vector<int> v_id;
  v_id.reserve(other->_points.size());
  for (int i = 0; i < other->_points.size(); i++) {
    v_id.push_back(add_vertex(other->_points[i], other->_colors[i]));
  } 

  for (auto const& value: other->_triangles) {
    add_triangle(v_id[value[0]], v_id[value[1]], v_id[value[2]]);
  } 
}

/**
 * Sets the color of all vertices to c
 */
void GeomStore::
set_color(LVecBase4f color) {
  for (int i = 0; i < _colors.size(); i++) {
    _colors[i] = color;
  }
}

/**
 * Sets the color of all vertices in filter to c
 */
void GeomStore::
set_color(LVecBase4f color, PTA(int) selection) {
  for (int i = 0; i < selection.size(); i++) {
    _colors[selection[i]] = color;
  }
}

/**
 * Sets the vertex color to the corresponding unit length vector
 */
void GeomStore::
normals_as_color() {
  for (int i = 0; i < _points.size(); i++) {
    LVecBase3f n = _points[i].normalized() * 0.5f + 0.5f;
    LVecBase4f c = LVecBase4f(n, 1.0f);
    _colors[i] = c;
  }
}

/**
 * Sets all vertex positions to unit length
 */
void GeomStore::
to_unit_sphere() {
  for (int i = 0; i < _points.size(); i++) {
    _points[i] = _points[i].normalized();
  }
}

/**
 * Returns a Panda3D GeomNode object from the current GeomStore w/o
 * individual vertex normal (flat shading).
 */
PT(GeomNode) GeomStore::
get_p3d_geom_node(string name) {
  const float div_three = 1.0f / 3.0f;
  PT(GeomVertexData) vertex_data = new GeomVertexData(
    name,
    GeomVertexFormat::get_v3c4(),
    Geom::UH_static
  );
  vertex_data->set_num_rows(_triangles.size() * 3);
  GeomVertexWriter vertex_writer = GeomVertexWriter(vertex_data, "vertex");
  GeomVertexWriter color_writer = GeomVertexWriter(vertex_data, "color");
  
  for (int i = 0; i < _triangles.size(); i++) {
    vertex_writer.add_data3f(_points[_triangles[i][0]]);
    vertex_writer.add_data3f(_points[_triangles[i][1]]);
    vertex_writer.add_data3f(_points[_triangles[i][2]]);
    
    LVecBase4f c = LVecBase4f(0.0f);
    c += (LVecBase4f) _colors[_triangles[i][0]];
    c += (LVecBase4f) _colors[_triangles[i][1]];
    c += (LVecBase4f) _colors[_triangles[i][2]];
    c *= div_three;
    color_writer.add_data4f(c);
    color_writer.add_data4f(c);
    color_writer.add_data4f(c);
  }

  PT(Geom) geom = new Geom(vertex_data);
  PT(GeomTriangles) triangle = new GeomTriangles(Geom::UH_static);
  for (int i = 0; i < _triangles.size() * 3; i++) {
    triangle->add_vertex(i);
  }
  geom->add_primitive(triangle);
  PT(GeomNode) node = new GeomNode(name);
  node->add_geom(geom);
  return node;
}

/**
 * Returns all triangle indices from this instance of GeomStore.
 */
PTA_LVecBase3i GeomStore::
get_triangle_indices() {
  return _triangles;
}

/**
 * Returns the vertex with id `vid`.
 */
LVecBase3f GeomStore::
get_vertex(int vid) {
  nassertr(vid >= 0 && vid < _points.size(), LVecBase3f(-1.0f));
  return _points[vid];
}

/**
 * Overloaded Operators for direct vertex manipulation
 * TODO: Implement spacial filters (bounding box/sphere) and flags
 *       to set as to which vector the operation gets executed on.
 */

int GeomStore::
operator + (float v) {
  if (v == 0) {
    return 0;
  }
  for (int i = 0; i < _points.size(); i++) {
    _points[i] += v;
  }
  return 0;
}

int GeomStore::
operator + (LVecBase3f v) {
  if (v == LVecBase3f(0.0f)) {
    return 0;
  }
  for (int i = 0; i < _points.size(); i++) {
    _points[i] += v;
  }
  return 0;
}

int GeomStore::
operator - (float v) {
  if (v == 0) {
    return 0;
  }
  for (int i = 0; i < _points.size(); i++) {
    _points[i] -= v;
  }
  return 0;
}

int GeomStore::
operator - (LVecBase3f v) {
  if (v == LVecBase3f(0.0f)) {
    return 0;
  }
  for (int i = 0; i < _points.size(); i++) {
    _points[i] -= v;
  }
  return 0;
}

int GeomStore::
operator * (float v) {
  for (int i = 0; i < _points.size(); i++) {
    _points[i] *= v;
  }
  return 0;
}

int GeomStore::
operator * (LVecBase3f v) {
  for (int i = 0; i < _points.size(); i++) {
    _points[i].componentwise_mult(v);
  }
  return 0;
}

int GeomStore::
operator / (float v) {
  assert(v != 0);
  for (int i = 0; i < _points.size(); i++) {
    _points[i] /= v;
  }
  return 0;
}

int GeomStore::
operator / (LVecBase3f v) {
  LVecBase3 dv = LVecBase3(1.0f / v.get_x(), 1.0f / v.get_y(), 1.0f / v.get_z());
  for (int i = 0; i < _points.size(); i++) {
    _points[i].componentwise_mult(dv);
  }
  return 0;
}
