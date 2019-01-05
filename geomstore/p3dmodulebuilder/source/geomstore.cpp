#include <iostream>
#include <vector>
#include <assert.h>

#include "geomstore.h"



GeomStore::
GeomStore() {
  my_vec.push_back(1.0f);
  my_vec.push_back(2.0f);
  my_vec.push_back(3.0f);
}

GeomStore::
~GeomStore() {
}

int GeomStore::
add_vertex(LVecBase3f v, LVecBase4f c) {
  int id = this->_vertices.size();
  Vertex *vert = &Vertex(v, c, id);
  this->_vertices.push_back(vert);
  // std::cout << my_vec[1] << std::endl;
  return id;
}

int GeomStore:: 
add_triangle(int v0, int v1, int v2) {
  int id = this->_triangles.size();
  Triangle *tri = &Triangle(
    this->_vertices[v0],
    this->_vertices[v1],
    this->_vertices[v2]
  );
  this->_triangles.push_back(tri);
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
 * leaving other intact
 */
void GeomStore::
extend(GeomStore *other) {
  int start_vertex_index = _vertices.size();

  for (uint i = 0; i < other->_vertices.size(); i++) {
    this->add_vertex(other->_vertices[i]->position, other->_vertices[i]->color);
  }
  
  for (uint i = 0; i < other->_triangles.size(); i++) {
    int v0 = other->_triangles[i]->vertex[0]->id + start_vertex_index;
    int v1 = other->_triangles[i]->vertex[1]->id + start_vertex_index;
    int v2 = other->_triangles[i]->vertex[2]->id + start_vertex_index;
    this->add_triangle(v0, v1, v2);
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
  for (uint i = 0; i < _vertices.size(); i++) {
    _vertices[i]->position += v;
  }
  return 0;
}

int GeomStore::
operator + (LVecBase3f v) {
  if (v == LVecBase3f(0.0f)) {
    return 0;
  }
  for (uint i = 0; i < _vertices.size(); i++) {
    _vertices[i]->position += v;
  }
  return 0;
}

int GeomStore::
operator - (float v) {
  if (v == 0) {
    return 0;
  }
  for (uint i = 0; i < _vertices.size(); i++) {
    _vertices[i]->position -= v;
  }
  return 0;
}

int GeomStore::
operator - (LVecBase3f v) {
  if (v == LVecBase3f(0.0f)) {
    return 0;
  }
  for (uint i = 0; i < _vertices.size(); i++) {
    _vertices[i]->position -= v;
  }
  return 0;
}

int GeomStore::
operator * (float v) {
  for (uint i = 0; i < _vertices.size(); i++) {
    _vertices[i]->position *= v;
  }
  return 0;
}

int GeomStore::
operator / (float v) {
  assert(v != 0);
  for (uint i = 0; i < _vertices.size(); i++) {
    _vertices[i]->position /= v;
  }
  return 0;
}

/**
 * This is used to implement the buffer protocol, in order to allow efficient
 * access to vertex data through a Python multiview object.
 */
int GeomStore::
__getbuffer__(PyObject *self, Py_buffer *view, int flags) {
#if PY_VERSION_HEX >= 0x02060000
  int row_size;
  bool pad_fmt;

  if ((flags & PyBUF_STRIDES) == PyBUF_STRIDES) {
    // The consumer is fine with having a stride value.
    row_size = my_vec.size() * sizeof(float);
    pad_fmt = false;
  } else {
    // The consumer expects a contiguous buffer.  Give the stride as row size,
    // and pad the format with extra bytes.
    row_size = sizeof(float);
    pad_fmt = true;
  }

  InternalBufferData *data = new InternalBufferData;
  data->_handle = my_vec;
  data->_num_rows = my_vec.size();
  data->_stride = sizeof(float);
  data->_format = "f";

  view->internal = (void*) data;

  if (self != nullptr) {
    Py_INCREF(self);
  }
  view->obj = self;
  view->buf = (void*) &my_vec[0];
  view->len = row_size * my_vec.size();
  view->readonly = 0;
  view->itemsize = row_size;
  view->format = nullptr;
  if ((flags & PyBUF_FORMAT) == PyBUF_FORMAT) {
    view->format = (char*) data->_format.c_str();
  }
  view->ndim = 1;
  view->shape = nullptr;
  if ((flags & PyBUF_ND) == PyBUF_ND) {
    view->shape = &data->_num_rows;
  }
  view->strides = nullptr;
  if ((flags & PyBUF_STRIDES) == PyBUF_STRIDES) {
    view->strides = &data->_stride;
  }
  view->suboffsets = nullptr;

  return 0;
#else
  return -1;
#endif
}

/**
 * Releases the buffer allocated by __getbuffer__.
 */
void GeomStore::
__releasebuffer__(PyObject *self, Py_buffer *view) const {
#if PY_VERSION_HEX >= 0x02060000
  // Note: PyBuffer_Release automatically decrements view->obj.
  InternalBufferData *data;
  data = (InternalBufferData *) view->internal;
  if (data == nullptr) {
    return;
  }
  delete data;
  view->internal = nullptr;
#endif
}

