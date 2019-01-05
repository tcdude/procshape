#include <iostream>
#include <vector>

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
  std::cout << my_vec[1] << std::endl;
  return 0;
}

int GeomStore:: 
add_triangle(int v0, int v1, int v2) {
  return 0;
}

void GeomStore::
subdivide_triangles(int s) {

}

void GeomStore::
subdivide_triangles_distance(float d) {

}

void GeomStore::
extend(GeomStore *other) {

}

int GeomStore::
operator+ (float v) {
  for (uint i=0; i < my_vec.size(); i++) {
    my_vec[i] += v;
  }
  return 0;
}


int GeomStore::
__getbuffer__(PyObject *self, Py_buffer *view, int flags) {
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
}

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

