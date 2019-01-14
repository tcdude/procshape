/**
 * @file geomstore.cpp
 * @author Tiziano Bettio
 * @date 2018-12-29
 */

#include "geomstore.h"


using namespace std;

/**
 * Constructor
 * Pass v_reserve and t_reserve to specify how much memory is reserved.
 */
GeomStore::
GeomStore(int v_reserve, int t_reserve) {
  _v_reserve = v_reserve;
  _t_reserve = t_reserve;
  _vertices.reserve(v_reserve);
  _triangles.reserve(t_reserve);
  _vertex_positions.reserve(v_reserve);
  _colors.reserve(v_reserve);
}

GeomStore::
~GeomStore() {
}

/**
 * Add vertex with position v and color (1.0, 1.0, 1.0, 1.0) to the GeomStore.
 */
int GeomStore::
add_vertex(LVecBase3f point) {
  return add_vertex(point, LVecBase4f(1.0f));
}

/**
 * Add vertex with position v and color c to the GeomStore.
 * Returns the vertex id on success, -1 on error.
 */
int GeomStore::
add_vertex(LVecBase3f point, LVecBase4f color) {
  if (_vertices.size() == _v_reserve) {
    geomstore_cat->error() << "Maximum Vertices of " << _v_reserve << "reached"
      << endl;
  }
  nassertr(_vertices.size() < _v_reserve, -1);
  for (int i = 0; i < _vertices.size(); i++) {
    if (_vertex_positions[i] == point && _colors[i] == color) {
      // geomstore_cat->warning() << "Vertex already exists!" << endl;
      return i;
    }
  }
  _vertex_positions.push_back(point);
  _colors.push_back(color);
  Vertex* vert = new Vertex(
    &_vertex_positions.back(), 
    &_colors.back(), 
    -1);
  _vertices.push_back(vert);
  _vertices.back()->id = _vertices.size() - 1;
  return _vertices.back()->id;
}

/**
 * Add a triangle to the GeomStore.
 * Returns the triangle id on success, -1 on error.
 */
int GeomStore:: 
add_triangle(int v0, int v1, int v2) {
  nassertr(_triangles.size() < _t_reserve, -1);
  nassertr(v0 >= 0 && v1 >= 0 && v2 >= 0, -1);
  if (v0 >= _vertices.size() || v1 >= _vertices.size() || v2 >= _vertices.size()) {
    geomstore_cat->error() << "Vertex index/indices out of bounds! Got (" 
      << v0 << "/" << v1 << "/" << v2 << "). Current size is " 
      << _vertices.size() << endl;
  }
  nassertr(v0 < _vertices.size() && v1 < _vertices.size() && v2 < _vertices.size(), -1);
  int id = _triangles.size();
  Triangle* tri = new Triangle(
    _vertices[v0],
    _vertices[v1],
    _vertices[v2]);
  _triangles.push_back(tri);
  return id;
}

/**
 * Add a quad to the GeomStore. Creates two triangles with successive ids.
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
  GeomStore *g = new GeomStore(_vertices.size() + 4, _triangles.size() + 4);
  g->extend(this);
  LVecBase3f m = 1.0f;
  m[axis] = -1.0f;
  g->mult_vec(m);
  g->flip_faces();
  extend(g);
  return true;
}

bool GeomStore::
flip_faces() {
  for (int i = 0; i < _triangles.size(); i++) {
    nassertr(_triangles[i]->flip(), false);
  }
  return true;
}

/**
 * Subdivide triangles s times. This is useful when the triangles already are
 * approximately the same size over the entire GeomStore.
 */
void GeomStore::
subdivide_triangles(int subdivisions) {
  int current_size;
  for (int j = 0; j < subdivisions; j++) {
    current_size = _triangles.size();
    for (int i = 0; i < current_size; i++) {
      nassertv(subdivide(_triangles[i]) == 0);
    }
  }
}

/**
 * Subdivides triangles until every hypothenuse is <= d (length in model view).
 * Use this when the triangles in GeomStore are of mixed size. Less efficient 
 * than subdivide_triangles(int s).
 */
void GeomStore::
subdivide_triangles_distance(float target_distance) {
  int subdivisions;
  int vert_id;
  int end_id;
  int last_id;
  do {
    subdivisions = 0;
    Triangle* it = *max_element(begin(_triangles), end(_triangles));
    if (it->_longest_edge_length > target_distance) {
      nassertv(subdivide(it) == 0);
      subdivisions++;
    }
  } while (subdivisions);
}

/**
 * Subdivides triangles until every hypothenuse is <= d (length in model view)
 * while also performing the following steps after each subdivision:
 *  1. normalize the new vertex to unit length
 *  2. multiply the vector by bb (Bounding Box)
 */
void GeomStore::
subdivide_triangles_spheroid(float target_distance, LVecBase3f bb) {
  int subdivisions;
  int vert_id;
  int end_id;
  int last_id;
  do {
    subdivisions = 0;
    Triangle* it = *max_element(begin(_triangles), end(_triangles));
    if (it->_longest_edge_length > target_distance) {
      nassertv(subdivide(it, true, bb) == 0);
      subdivisions++;
    }
  } while (subdivisions);
}

/**
 * Does the actual subdivision of the triangle.
 */
int GeomStore::
subdivide(Triangle* t, bool s, LVecBase3f bb) {
  LVecBase3f point = t->get_split_point();
  LVecBase4f color = t->get_split_color();
  if (s) {
    point = point.normalized();
    point.componentwise_mult(bb);
  }
  int vert_id = add_vertex(point, color);
  int end_id = (t->_longest_edge_index + 1) % 3;
  int last_id = (end_id + 1) % 3;
  nassertr(add_triangle(vert_id, t->vertex[end_id]->id, t->vertex[last_id]->id), -1);
  nassertr(t->replace_vertex(t->vertex[end_id], _vertices[vert_id]), -1);
  return 0;
}

/**
 * Extend this GeomStore with all Vertices and Triangles from other while
 * leaving other intact (New objects are being generated!).
 */
void GeomStore::
extend(GeomStore *other) {
  int start_vertex_index = _vertices.size();
  vector<int> v_id(other->_vertices.size());
  for (int i = 0; i < other->_vertices.size(); i++) {
    LVecBase3f _pos = *other->_vertices[i]->position;
    UnalignedLVecBase4f _col = *other->_vertices[i]->color;
    v_id[i] = add_vertex(_pos, _col);
  }
  
  for (int i = 0; i < other->_triangles.size(); i++) {
    int v0 = v_id[other->_triangles[i]->vertex[0]->id];
    int v1 = v_id[other->_triangles[i]->vertex[1]->id];
    int v2 = v_id[other->_triangles[i]->vertex[2]->id];
    add_triangle(v0, v1, v2);
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
  for (int i = 0; i < _vertex_positions.size(); i++) {
    LVecBase3f n = _vertex_positions[i].normalized() * 0.5f + 0.5f;
    LVecBase4f c = LVecBase4f(n, 1.0f);
    _colors[i] = c;
  }
}

/**
 * Sets all vertex positions to unit length
 */
void GeomStore::
to_unit_sphere() {
  for (int i = 0; i < _vertex_positions.size(); i++) {
    _vertex_positions[i] = _vertex_positions[i].normalized();
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
    vertex_writer.add_data3f(*_triangles[i]->vertex[0]->position);
    vertex_writer.add_data3f(*_triangles[i]->vertex[1]->position);
    vertex_writer.add_data3f(*_triangles[i]->vertex[2]->position);
    
    LVecBase4f c = LVecBase4f(0.0f);
    c += (LVecBase4f) *_triangles[i]->vertex[0]->color;
    c += (LVecBase4f) *_triangles[i]->vertex[1]->color;
    c += (LVecBase4f) *_triangles[i]->vertex[2]->color;
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
  PTA_LVecBase3i t_i;
  t_i.reserve(_triangles.size());
  for (int i = 0; i < _triangles.size(); i++) {
    LVecBase3i idx = {_triangles[i]->vertex[0]->id,
                      _triangles[i]->vertex[1]->id, 
                      _triangles[i]->vertex[2]->id};
    t_i.push_back(idx);
  }
  return t_i;
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
  for (int i = 0; i < _vertex_positions.size(); i++) {
    _vertex_positions[i] += v;
  }
  return 0;
}

int GeomStore::
operator + (LVecBase3f v) {
  if (v == LVecBase3f(0.0f)) {
    return 0;
  }
  for (int i = 0; i < _vertex_positions.size(); i++) {
    _vertex_positions[i] += v;
  }
  return 0;
}

int GeomStore::
operator - (float v) {
  if (v == 0) {
    return 0;
  }
  for (int i = 0; i < _vertex_positions.size(); i++) {
    _vertex_positions[i] -= v;
  }
  return 0;
}

int GeomStore::
operator - (LVecBase3f v) {
  if (v == LVecBase3f(0.0f)) {
    return 0;
  }
  for (int i = 0; i < _vertex_positions.size(); i++) {
    _vertex_positions[i] -= v;
  }
  return 0;
}

int GeomStore::
operator * (float v) {
  for (int i = 0; i < _vertex_positions.size(); i++) {
    _vertex_positions[i] *= v;
  }
  return 0;
}

int GeomStore::
operator * (LVecBase3f v) {
  mult_vec(v);
  return 0;
}

void GeomStore::
mult_vec(LVecBase3f v) {
  for (int i = 0; i < _vertex_positions.size(); i++) {
    _vertex_positions[i].componentwise_mult(v);
  }
}

int GeomStore::
operator / (float v) {
  assert(v != 0);
  for (int i = 0; i < _vertex_positions.size(); i++) {
    _vertex_positions[i] /= v;
  }
  return 0;
}

int GeomStore::
operator / (LVecBase3f v) {
  LVecBase3 dv = LVecBase3(1.0f / v.get_x(), 1.0f / v.get_y(), 1.0f / v.get_z());
  for (int i = 0; i < _vertex_positions.size(); i++) {
    _vertex_positions[i].componentwise_mult(dv);
  }
  return 0;
}

