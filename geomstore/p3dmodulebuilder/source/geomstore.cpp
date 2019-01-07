/**
 * @file geomstore.cpp
 * @author Tiziano Bettio
 * @date 2018-12-29
 */

#include "geomstore.h"


using namespace std;

GeomStore::
GeomStore() {
  //_vertices.reserve((int)1e6);
  //_triangles.reserve((int)1e6);
  _vertex_positions.reserve((int)1e6);
  _colors.reserve((int)1e6);
  _triangle_indices.reserve((int)1e6);
}

GeomStore::
~GeomStore() {
}

/**
 * Add vertex with position v and color (1.0, 1.0, 1.0, 1.0) to the GeomStore.
 */
int GeomStore::
add_vertex(LVecBase3f v) {
  for (int i = 0; i < _vertices.size(); i++) {
    if (_vertex_positions[i] == v && _colors[i] == LVecBase4f(1.0)) {
      return i;
    }
  }
  int id = _vertices.size();
  _vertex_positions.push_back(v);
  _colors.push_back(LVecBase4f(1.0f));
  Vertex* vert = new Vertex(
    &_vertex_positions.back(), 
    &_colors.back(), 
    id);
  _vertices.push_back(vert);
  return id;
}

/**
 * Add vertex with position v and color c to the GeomStore.
 */
int GeomStore::
add_vertex(LVecBase3f v, LVecBase4f c) {
  for (int i = 0; i < _vertices.size(); i++) {
    if (_vertex_positions[i] == v && _colors[i] == c) {
      return i;
    }
  }
  int id = _vertices.size();
  _vertex_positions.push_back(v);
  _colors.push_back(c);
  Vertex* vert = new Vertex(
    &_vertex_positions.back(), 
    &_colors.back(), 
    id);
  _vertices.push_back(vert);
  return id;
}

/**
 * Add a triangle to the GeomStore.
 */
int GeomStore:: 
add_triangle(int v0, int v1, int v2) {
  nassertr(v0 >= 0 && v1 >= 0 && v2 >= 0, -1);
  nassertr(v0 < _vertices.size() && v1 < _vertices.size() && v2 < _vertices.size(), -1);
  int id = _triangles.size();
  Triangle* tri = new Triangle(
    _vertices[v0],
    _vertices[v1],
    _vertices[v2]);
  _triangles.push_back(tri);
  LVecBase3i indices = LVecBase3i(v0, v1, v2);
  _triangle_indices.push_back(indices);
  return id;
}

/**
 * Subdivide triangles s times. This is useful when the triangles already are
 * approximately the same size over the entire GeomStore.
 */
void GeomStore::
subdivide_triangles(int s) {
  for (int j = 0; j < s; j++) {
    subdivide();
  }
}

/**
 * Subdivides triangles until every hypothenuse is <= d (length in model view).
 * Use this when the triangles in GeomStore are of mixed size. Less efficient 
 * than subdivide_triangles(int s).
 */
void GeomStore::
subdivide_triangles_distance(float d) {
  while (subdivide(d));
}

/**
 * Does the actual subdivision of the triangles.
 */
int GeomStore::
subdivide(float d) {
  int current_size = _triangles.size();
  int subdivisions = 0;
  for (int i = 0; i < current_size; i++) {
    if (d == 0.0f || (d > 0.0f && _triangles[i]->_longest_edge_length > d)) {
      int start_id = _triangles[i]->_longest_edge_index;
      int end_id = (start_id + 1) % 3;
      int last_id = (start_id + 2) % 3;
      LVecBase3f v_new = *_triangles[i]->vertex[end_id]->position - 
        *_triangles[i]->vertex[start_id]->position;
      LVecBase3f s_pos = *_triangles[i]->vertex[start_id]->position;
      v_new = s_pos + v_new * 0.5f;
      LVecBase4f c_new = (LVecBase4f) _colors[_triangles[i]->vertex[end_id]->id]
        + (LVecBase4f) _colors[_triangles[i]->vertex[start_id]->id];
      c_new *= 0.5;
      int vert_id = add_vertex(v_new, (UnalignedLVecBase4f) c_new);
      add_triangle(vert_id, _triangles[i]->vertex[end_id]->id, 
        _triangles[i]->vertex[last_id]->id);
      _triangles[i]->replace_vertex(_triangles[i]->vertex[end_id], 
        _vertices[vert_id]);
      _triangle_indices[i][end_id] = vert_id;
      subdivisions++;
    }
  }
  return subdivisions;
}

/**
 * Extend this GeomStore with all Vertices and Triangles from other while
 * leaving other intact (New objects are being generated!).
 */
void GeomStore::
extend(GeomStore *other) {
  int start_vertex_index = _vertices.size();

  for (uint i = 0; i < other->_vertices.size(); i++) {
    LVecBase3f _pos = *other->_vertices[i]->position;
    UnalignedLVecBase4f _col = *other->_vertices[i]->color;
    add_vertex(_pos, _col);
  }
  
  for (uint i = 0; i < other->_triangles.size(); i++) {
    int v0 = other->_triangles[i]->vertex[0]->id + start_vertex_index;
    int v1 = other->_triangles[i]->vertex[1]->id + start_vertex_index;
    int v2 = other->_triangles[i]->vertex[2]->id + start_vertex_index;
    add_triangle(v0, v1, v2);
  }
}

/**
 * Sets the color of all vertices to c
 */
void GeomStore::
set_color(LVecBase4f c) {
  for (int i = 0; i < _colors.size(); i++) {
    _colors[i] = c;
  }
}

/**
 * Sets the color of all vertices in filter to c
 */
void GeomStore::
set_color(LVecBase4f c, PTA(int) filter) {
  for (int i = 0; i < filter.size(); i++) {
    _colors[filter[i]] = c;
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
    _vertex_positions[i].normalize();
  }
}

/**
 * Returns a Panda3D GeomNode object from the current GeomStore w/o
 * individual vertex normal (flat shading).
 */
PT(GeomNode) GeomStore::
get_p3d_geom_node(string name) {
  /*        
  vertex_data = GeomVertexData(
      name,
      GeomVertexFormat.get_v3c4(),
      Geom.UH_static
  )
  total_triangles = len(self.__triangles__)

  # Every triangle gets its unique set of vertices for flat shading
  num_rows = total_triangles * 3
  vertex_data.set_num_rows(num_rows)
  vertex_writer = GeomVertexWriter(vertex_data, 'vertex')
  color_writer = GeomVertexWriter(vertex_data, 'color')
  for i in range(total_triangles):
      triangle = self.vertices[self.triangles[i]]
      vertex_writer.add_data3f(*triangle[0])
      vertex_writer.add_data3f(*triangle[1])
      vertex_writer.add_data3f(*triangle[2])
      triangle_color = self.colors[self.triangles[i]].sum(axis=0) / 3
      for _ in [0, 0, 0]:
          color_writer.add_data4f(*triangle_color)

  geom = Geom(vertex_data)
  for i in range(total_triangles):
      triangle = GeomTriangles(Geom.UH_static)
      first = i * 3
      triangle.add_vertex(first)
      triangle.add_vertex(first + 1)
      triangle.add_vertex(first + 2)
      geom.add_primitive(triangle)
  node = GeomNode(name)
  node.add_geom(geom)
  return node
*/
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
  for (int i = 0; i < _triangles.size(); i++) {
    triangle->add_vertex(_triangle_indices[i][0]);
    triangle->add_vertex(_triangle_indices[i][1]);
    triangle->add_vertex(_triangle_indices[i][2]);
  }
  PT(GeomNode) node = new GeomNode(name);
  node->add_geom(geom);
  return node;
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
  for (int i = 0; i < _vertex_positions.size(); i++) {
    _vertex_positions[i].componentwise_mult(v);
  }
  return 0;
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

