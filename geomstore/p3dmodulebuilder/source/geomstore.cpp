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

int GeomStore:: 
add_triangle(int v0, int v1, int v2) {
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
 * Overloaded Operators for direct vertex manipulation
 * TODO: Consider a filter mechanism in the class
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
operator / (float v) {
  assert(v != 0);
  for (int i = 0; i < _vertex_positions.size(); i++) {
    _vertex_positions[i] /= v;
  }
  return 0;
}

/**
 * Debugging methods to test the content of the various PTAs
 */
void GeomStore::
print_vertices() {
  print_pta(_vertex_positions);
}

void GeomStore::
print_triangles() {
  print_pta(_triangle_indices);
}

void GeomStore::
print_colors() {
  print_pta(_colors);
}

void GeomStore::
print_pta(PTA_LVecBase3f _pta) {
  cout << "[";
  for (int i = 0; i < _pta.size(); i++) {
    if (i != 0) {
      cout << " ";
    }
    cout << "[" << _pta[i] << "]";
    if (i < _pta.size() - 1) {
      cout << endl;
    }
  }
  cout << "]" << endl;
}

void GeomStore::
print_pta(PTA_LVecBase3i _pta) {
  cout << "[";
  for (int i = 0; i < _pta.size(); i++) {
    if (i != 0) {
      cout << " ";
    }
    cout << "[" << _pta[i] << "]";
    if (i < _pta.size() - 1) {
      cout << endl;
    }
  }
  cout << "]" << endl;
}

void GeomStore::
print_pta(PTA_LVecBase4f _pta) {
  cout << "[";
  for (int i = 0; i < _pta.size(); i++) {
    if (i != 0) {
      cout << " ";
    }
    LVecBase4f v = (LVecBase4f) _pta[i];
    cout << "[" << _pta[i] << "]";
    if (i < _pta.size() - 1) {
      cout << endl;
    }
  }
  cout << "]" << endl;
}

