/**
 * @file geomstore.cpp
 * @author Tiziano Bettio
 * @date 2018-12-29
 */

#include "geomstore.h"


using namespace std;

GeomStore::
GeomStore() {
  /*_vertex_positions.push_back(LVecBase3f(1.0f, 0.0f, 0.0f));
  _vertex_positions.push_back(LVecBase3f(0.0f, 1.0f, 0.0f));
  _vertex_positions.push_back(LVecBase3f(0.0f, 0.0f, 1.0f));
  _triangle_indices.push_back(LVecBase3i(0, 1, 2));
  _colors.push_back(LVecBase4f(1.0f, 0.0f, 0.0f, 1.0f));
  _colors.push_back(LVecBase4f(0.0f, 1.0f, 0.0f, 1.0f));
  _colors.push_back(LVecBase4f(0.0f, 0.0f, 1.0f, 1.0f));*/
}

GeomStore::
~GeomStore() {
}

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

int GeomStore::
add_vertex(LVecBase3f v, LVecBase4f c) {
  int id = _vertices.size();
  _vertex_positions.push_back(v);
  _colors.push_back(c);
  Vertex vert = Vertex(
    &_vertex_positions.back(), 
    &_colors.back(), 
    id
    );
  _vertices.push_back(&vert);
  return id;
}

int GeomStore:: 
add_triangle(int v0, int v1, int v2) {
  int id = _triangles.size();
  Triangle tri = Triangle(
    _vertices[v0],
    _vertices[v1],
    _vertices[v2]
  );
  _triangles.push_back(&tri);
  return id;
}

void GeomStore::
subdivide_triangles(int s) {

}

void GeomStore::
subdivide_triangles_distance(float d) {

}

/**
 * Extend this GeomStore with all Vertices and Triangles from other while
 * leaving other intact (New objects are being generated!)
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
