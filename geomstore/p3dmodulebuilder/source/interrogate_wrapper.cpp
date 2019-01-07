/*
 * This file was generated by:
 * /usr/bin/interrogate -fnames -string -refcount -assert -python-native -S/usr/include/panda3d/parser-inc -S/usr/include/panda3d/ -srcdir . -oc interrogate_wrapper.cpp -od interrogate.in -module geomstore -library geomstore -nomangle -DINTERROGATE -DCPPPARSER -D__STDC__=1 -D__cplusplus=201103L -D__attribute__\(x\)= -D__i386__ ./triangle.h ./common.cpp ./vertex.h ./vertex.cpp ./config_module.h ./common.h ./geomstore.h ./config_module.cpp ./triangle.cpp ./geomstore.cpp
 *
 */

#include "dtoolbase.h"
#include "interrogate_request.h"
#include "dconfig.h"
#include "pnotify.h"
#include <sstream>
#define PANDA_LIBRARY_NAME_geomstore
#include "py_panda.h"
#include "extension.h"
#include "dcast.h"

#include "./common.h"
#include "./config_module.h"
#include "./geomstore.h"
#include "./triangle.h"
#include "./vertex.h"
#include "Python.h"
#include "common.h"
#include "config_module.h"
#include "dconfig.h"
#include "geomstore.h"
#include <iostream>
#include "luse.h"
#include "notifyCategoryProxy.h"
#include "pandabase.h"
#include "pnotify.h"
#include "pta_LVecBase3.h"
#include "pta_LVecBase4.h"
#include "triangle.h"
#include <vector>
#include "vertex.h"

#undef _POSIX_C_SOURCE
#undef _XOPEN_SOURCE
#define PY_SSIZE_T_CLEAN 1

#if PYTHON_FRAMEWORK
  #include <Python/Python.h>
#else
  #include "Python.h"
#endif

/**
 * Forward declarations for top-level class GeomStore
 */
typedef GeomStore GeomStore_localtype;
Define_Module_Class(geomstore, GeomStore, GeomStore_localtype, GeomStore);
static struct Dtool_PyTypedObject *const Dtool_Ptr_GeomStore = &Dtool_GeomStore;
static void Dtool_PyModuleClassInit_GeomStore(PyObject *module);

/**
 * Declarations for exported classes
 */
static const Dtool_TypeDef exports[] = {
  {"GeomStore", &Dtool_GeomStore},
  {nullptr, nullptr},
};

/**
 * Extern declarations for imported classes
 */
#ifndef LINK_ALL_STATIC
static Dtool_TypeDef imports[] = {
  {"LVecBase3f", nullptr},
#define Dtool_Ptr_LVecBase3f (imports[0].type)
  {"LVecBase4f", nullptr},
#define Dtool_Ptr_LVecBase4f (imports[1].type)
  {"PointerToArray< LVecBase3f >", nullptr},
#define Dtool_Ptr_PointerToArray_LVecBase3f (imports[2].type)
  {"PointerToArray< LVecBase3i >", nullptr},
#define Dtool_Ptr_PointerToArray_LVecBase3i (imports[3].type)
  {"PointerToArray< UnalignedLVecBase4f >", nullptr},
#define Dtool_Ptr_PointerToArray_UnalignedLVecBase4f (imports[4].type)
  {nullptr, nullptr},
};
#endif

// LVecBase3f
#ifndef LINK_ALL_STATIC
inline static LVecBase3f *Dtool_Coerce_LVecBase3f(PyObject *args, LVecBase3f &coerced) {
  nassertr(Dtool_Ptr_LVecBase3f != nullptr, nullptr);
  nassertr(Dtool_Ptr_LVecBase3f->_Dtool_Coerce != nullptr, nullptr);
  return ((LVecBase3f *(*)(PyObject *, LVecBase3f &))Dtool_Ptr_LVecBase3f->_Dtool_Coerce)(args, coerced);
}
#else
extern struct Dtool_PyTypedObject Dtool_LVecBase3f;
static struct Dtool_PyTypedObject *const Dtool_Ptr_LVecBase3f = &Dtool_LVecBase3f;
extern LVecBase3f *Dtool_Coerce_LVecBase3f(PyObject *args, LVecBase3f &coerced);
#endif
// LVecBase4f
#ifndef LINK_ALL_STATIC
inline static LVecBase4f *Dtool_Coerce_LVecBase4f(PyObject *args, LVecBase4f &coerced) {
  nassertr(Dtool_Ptr_LVecBase4f != nullptr, nullptr);
  nassertr(Dtool_Ptr_LVecBase4f->_Dtool_Coerce != nullptr, nullptr);
  return ((LVecBase4f *(*)(PyObject *, LVecBase4f &))Dtool_Ptr_LVecBase4f->_Dtool_Coerce)(args, coerced);
}
#else
extern struct Dtool_PyTypedObject Dtool_LVecBase4f;
static struct Dtool_PyTypedObject *const Dtool_Ptr_LVecBase4f = &Dtool_LVecBase4f;
extern LVecBase4f *Dtool_Coerce_LVecBase4f(PyObject *args, LVecBase4f &coerced);
#endif
// PointerToArray< LVecBase3f >
#ifndef LINK_ALL_STATIC
inline static PointerToArray< LVecBase3f > *Dtool_Coerce_PointerToArray_LVecBase3f(PyObject *args, PointerToArray< LVecBase3f > &coerced) {
  nassertr(Dtool_Ptr_PointerToArray_LVecBase3f != nullptr, nullptr);
  nassertr(Dtool_Ptr_PointerToArray_LVecBase3f->_Dtool_Coerce != nullptr, nullptr);
  return ((PointerToArray< LVecBase3f > *(*)(PyObject *, PointerToArray< LVecBase3f > &))Dtool_Ptr_PointerToArray_LVecBase3f->_Dtool_Coerce)(args, coerced);
}
#else
extern struct Dtool_PyTypedObject Dtool_PointerToArray_LVecBase3f;
static struct Dtool_PyTypedObject *const Dtool_Ptr_PointerToArray_LVecBase3f = &Dtool_PointerToArray_LVecBase3f;
extern PointerToArray< LVecBase3f > *Dtool_Coerce_PointerToArray_LVecBase3f(PyObject *args, PointerToArray< LVecBase3f > &coerced);
#endif
// PointerToArray< LVecBase3i >
#ifndef LINK_ALL_STATIC
inline static PointerToArray< LVecBase3i > *Dtool_Coerce_PointerToArray_LVecBase3i(PyObject *args, PointerToArray< LVecBase3i > &coerced) {
  nassertr(Dtool_Ptr_PointerToArray_LVecBase3i != nullptr, nullptr);
  nassertr(Dtool_Ptr_PointerToArray_LVecBase3i->_Dtool_Coerce != nullptr, nullptr);
  return ((PointerToArray< LVecBase3i > *(*)(PyObject *, PointerToArray< LVecBase3i > &))Dtool_Ptr_PointerToArray_LVecBase3i->_Dtool_Coerce)(args, coerced);
}
#else
extern struct Dtool_PyTypedObject Dtool_PointerToArray_LVecBase3i;
static struct Dtool_PyTypedObject *const Dtool_Ptr_PointerToArray_LVecBase3i = &Dtool_PointerToArray_LVecBase3i;
extern PointerToArray< LVecBase3i > *Dtool_Coerce_PointerToArray_LVecBase3i(PyObject *args, PointerToArray< LVecBase3i > &coerced);
#endif
// PointerToArray< UnalignedLVecBase4f >
#ifndef LINK_ALL_STATIC
inline static PointerToArray< UnalignedLVecBase4f > *Dtool_Coerce_PointerToArray_UnalignedLVecBase4f(PyObject *args, PointerToArray< UnalignedLVecBase4f > &coerced) {
  nassertr(Dtool_Ptr_PointerToArray_UnalignedLVecBase4f != nullptr, nullptr);
  nassertr(Dtool_Ptr_PointerToArray_UnalignedLVecBase4f->_Dtool_Coerce != nullptr, nullptr);
  return ((PointerToArray< UnalignedLVecBase4f > *(*)(PyObject *, PointerToArray< UnalignedLVecBase4f > &))Dtool_Ptr_PointerToArray_UnalignedLVecBase4f->_Dtool_Coerce)(args, coerced);
}
#else
extern struct Dtool_PyTypedObject Dtool_PointerToArray_UnalignedLVecBase4f;
static struct Dtool_PyTypedObject *const Dtool_Ptr_PointerToArray_UnalignedLVecBase4f = &Dtool_PointerToArray_UnalignedLVecBase4f;
extern PointerToArray< UnalignedLVecBase4f > *Dtool_Coerce_PointerToArray_UnalignedLVecBase4f(PyObject *args, PointerToArray< UnalignedLVecBase4f > &coerced);
#endif

/**
 * Python wrappers for global functions
 */
/**
 * Python wrappers for functions of class GeomStore
 */
/**
 * Python function wrapper for:
 * void GeomStore::print_vertices(void)
 */
static PyObject *Dtool_GeomStore_print_vertices_4(PyObject *self, PyObject *) {
  GeomStore *local_this = nullptr;
  if (!Dtool_Call_ExtractThisPointer_NonConst(self, Dtool_GeomStore, (void **)&local_this, "GeomStore.print_vertices")) {
    return nullptr;
  }
  // 1-void GeomStore::print_vertices(void)
  (*local_this).print_vertices();
  return Dtool_Return_None();
}

#ifndef NDEBUG
static const char *Dtool_GeomStore_print_vertices_4_comment =
  "C++ Interface:\n"
  "print_vertices(const GeomStore self)\n"
  "\n"
  "/**\n"
  " * Debugging methods to test the content of the various PTAs\n"
  " */";
#else
static const char *Dtool_GeomStore_print_vertices_4_comment = nullptr;
#endif

/**
 * Python function wrapper for:
 * void GeomStore::print_colors(void)
 */
static PyObject *Dtool_GeomStore_print_colors_5(PyObject *self, PyObject *) {
  GeomStore *local_this = nullptr;
  if (!Dtool_Call_ExtractThisPointer_NonConst(self, Dtool_GeomStore, (void **)&local_this, "GeomStore.print_colors")) {
    return nullptr;
  }
  // 1-void GeomStore::print_colors(void)
  (*local_this).print_colors();
  return Dtool_Return_None();
}

#ifndef NDEBUG
static const char *Dtool_GeomStore_print_colors_5_comment =
  "C++ Interface:\n"
  "print_colors(const GeomStore self)\n";
#else
static const char *Dtool_GeomStore_print_colors_5_comment = nullptr;
#endif

/**
 * Python function wrapper for:
 * void GeomStore::print_triangles(void)
 */
static PyObject *Dtool_GeomStore_print_triangles_6(PyObject *self, PyObject *) {
  GeomStore *local_this = nullptr;
  if (!Dtool_Call_ExtractThisPointer_NonConst(self, Dtool_GeomStore, (void **)&local_this, "GeomStore.print_triangles")) {
    return nullptr;
  }
  // 1-void GeomStore::print_triangles(void)
  (*local_this).print_triangles();
  return Dtool_Return_None();
}

#ifndef NDEBUG
static const char *Dtool_GeomStore_print_triangles_6_comment =
  "C++ Interface:\n"
  "print_triangles(const GeomStore self)\n";
#else
static const char *Dtool_GeomStore_print_triangles_6_comment = nullptr;
#endif

/**
 * Python function wrapper for:
 * int GeomStore::add_vertex(LVecBase3f v, LVecBase4f c)
 */
static PyObject *Dtool_GeomStore_add_vertex_7(PyObject *self, PyObject *args, PyObject *kwds) {
  GeomStore *local_this = nullptr;
  if (!Dtool_Call_ExtractThisPointer_NonConst(self, Dtool_GeomStore, (void **)&local_this, "GeomStore.add_vertex")) {
    return nullptr;
  }
  // 1-int GeomStore::add_vertex(LVecBase3f v, LVecBase4f c)
  PyObject *param1;
  PyObject *param2;
  static const char *keyword_list[] = {"v", "c", nullptr};
  if (PyArg_ParseTupleAndKeywords(args, kwds, "OO:add_vertex", (char **)keyword_list, &param1, &param2)) {
    LVecBase3f param1_local;
    LVecBase3f *param1_this = Dtool_Coerce_LVecBase3f(param1, param1_local);
    if (!(param1_this != nullptr)) {
      return Dtool_Raise_ArgTypeError(param1, 1, "GeomStore.add_vertex", "LVecBase3f");
    }
    LVecBase4f param2_local;
    LVecBase4f *param2_this = Dtool_Coerce_LVecBase4f(param2, param2_local);
    if (!(param2_this != nullptr)) {
      return Dtool_Raise_ArgTypeError(param2, 2, "GeomStore.add_vertex", "LVecBase4f");
    }
    int return_value = (*local_this).add_vertex(*param1_this, *param2_this);
    if (Dtool_CheckErrorOccurred()) {
      return nullptr;
    }
    return Dtool_WrapValue(return_value);
  }
  if (!_PyErr_OCCURRED()) {
    return Dtool_Raise_BadArgumentsError(
      "add_vertex(const GeomStore self, LVecBase3f v, LVecBase4f c)\n");
  }
  return nullptr;
}

#ifndef NDEBUG
static const char *Dtool_GeomStore_add_vertex_7_comment =
  "C++ Interface:\n"
  "add_vertex(const GeomStore self, LVecBase3f v, LVecBase4f c)\n";
#else
static const char *Dtool_GeomStore_add_vertex_7_comment = nullptr;
#endif

/**
 * Python function wrapper for:
 * int GeomStore::add_triangle(int v0, int v1, int v2)
 */
static PyObject *Dtool_GeomStore_add_triangle_8(PyObject *self, PyObject *args, PyObject *kwds) {
  GeomStore *local_this = nullptr;
  if (!Dtool_Call_ExtractThisPointer_NonConst(self, Dtool_GeomStore, (void **)&local_this, "GeomStore.add_triangle")) {
    return nullptr;
  }
  // 1-int GeomStore::add_triangle(int v0, int v1, int v2)
  int param1;
  int param2;
  int param3;
  static const char *keyword_list[] = {"v0", "v1", "v2", nullptr};
  if (PyArg_ParseTupleAndKeywords(args, kwds, "iii:add_triangle", (char **)keyword_list, &param1, &param2, &param3)) {
    int return_value = (*local_this).add_triangle((int)param1, (int)param2, (int)param3);
    if (Dtool_CheckErrorOccurred()) {
      return nullptr;
    }
    return Dtool_WrapValue(return_value);
  }
  if (!_PyErr_OCCURRED()) {
    return Dtool_Raise_BadArgumentsError(
      "add_triangle(const GeomStore self, int v0, int v1, int v2)\n");
  }
  return nullptr;
}

#ifndef NDEBUG
static const char *Dtool_GeomStore_add_triangle_8_comment =
  "C++ Interface:\n"
  "add_triangle(const GeomStore self, int v0, int v1, int v2)\n";
#else
static const char *Dtool_GeomStore_add_triangle_8_comment = nullptr;
#endif

/**
 * Python function wrapper for:
 * void GeomStore::subdivide_triangles(int s)
 */
static PyObject *Dtool_GeomStore_subdivide_triangles_9(PyObject *self, PyObject *arg) {
  GeomStore *local_this = nullptr;
  if (!Dtool_Call_ExtractThisPointer_NonConst(self, Dtool_GeomStore, (void **)&local_this, "GeomStore.subdivide_triangles")) {
    return nullptr;
  }
  // 1-void GeomStore::subdivide_triangles(int s)
  if (PyLongOrInt_Check(arg)) {
    long arg_val = PyLongOrInt_AS_LONG(arg);
#if (SIZEOF_LONG > SIZEOF_INT) && !defined(NDEBUG)
    if (arg_val < INT_MIN || arg_val > INT_MAX) {
      return PyErr_Format(PyExc_OverflowError,
                          "value %ld out of range for signed integer",
                          arg_val);
    }
#endif
    (*local_this).subdivide_triangles((int)arg_val);
    return Dtool_Return_None();
  }
  if (!_PyErr_OCCURRED()) {
    return Dtool_Raise_BadArgumentsError(
      "subdivide_triangles(const GeomStore self, int s)\n");
  }
  return nullptr;
}

#ifndef NDEBUG
static const char *Dtool_GeomStore_subdivide_triangles_9_comment =
  "C++ Interface:\n"
  "subdivide_triangles(const GeomStore self, int s)\n"
  "\n"
  "/**\n"
  " * Subdivide triangles s times. This is useful when the triangles already are\n"
  " * approximately the same size over the entire GeomStore.\n"
  " */";
#else
static const char *Dtool_GeomStore_subdivide_triangles_9_comment = nullptr;
#endif

/**
 * Python function wrapper for:
 * void GeomStore::subdivide_triangles_distance(float d)
 */
static PyObject *Dtool_GeomStore_subdivide_triangles_distance_10(PyObject *self, PyObject *arg) {
  GeomStore *local_this = nullptr;
  if (!Dtool_Call_ExtractThisPointer_NonConst(self, Dtool_GeomStore, (void **)&local_this, "GeomStore.subdivide_triangles_distance")) {
    return nullptr;
  }
  // 1-void GeomStore::subdivide_triangles_distance(float d)
  if (PyNumber_Check(arg)) {
    (*local_this).subdivide_triangles_distance((float)PyFloat_AsDouble(arg));
    return Dtool_Return_None();
  }
  if (!_PyErr_OCCURRED()) {
    return Dtool_Raise_BadArgumentsError(
      "subdivide_triangles_distance(const GeomStore self, float d)\n");
  }
  return nullptr;
}

#ifndef NDEBUG
static const char *Dtool_GeomStore_subdivide_triangles_distance_10_comment =
  "C++ Interface:\n"
  "subdivide_triangles_distance(const GeomStore self, float d)\n"
  "\n"
  "/**\n"
  " * Subdivides triangles until every hypothenuse is <= d (length in model view).\n"
  " * Use this when the triangles in GeomStore are of mixed size. Less efficient \n"
  " * than subdivide_triangles(int s).\n"
  " */";
#else
static const char *Dtool_GeomStore_subdivide_triangles_distance_10_comment = nullptr;
#endif

/**
 * Python function wrapper for:
 * void GeomStore::extend(GeomStore *other)
 */
static PyObject *Dtool_GeomStore_extend_11(PyObject *self, PyObject *arg) {
  GeomStore *local_this = nullptr;
  if (!Dtool_Call_ExtractThisPointer_NonConst(self, Dtool_GeomStore, (void **)&local_this, "GeomStore.extend")) {
    return nullptr;
  }
  // 1-void GeomStore::extend(GeomStore *other)
  GeomStore *arg_this = (GeomStore *)DTOOL_Call_GetPointerThisClass(arg, Dtool_Ptr_GeomStore, 1, "GeomStore.extend", false, true);
  if (arg_this != nullptr) {
    (*local_this).extend(arg_this);
    return Dtool_Return_None();
  }
  if (!_PyErr_OCCURRED()) {
    return Dtool_Raise_BadArgumentsError(
      "extend(const GeomStore self, GeomStore other)\n");
  }
  return nullptr;
}

#ifndef NDEBUG
static const char *Dtool_GeomStore_extend_11_comment =
  "C++ Interface:\n"
  "extend(const GeomStore self, GeomStore other)\n"
  "\n"
  "/**\n"
  " * Extend this GeomStore with all Vertices and Triangles from other while\n"
  " * leaving other intact (New objects are being generated!).\n"
  " */";
#else
static const char *Dtool_GeomStore_extend_11_comment = nullptr;
#endif

static PyObject *Dtool_GeomStore__vertex_positions_Getter(PyObject *self, void *) {
  const GeomStore *local_this = nullptr;
  if (!Dtool_Call_ExtractThisPointer(self, Dtool_GeomStore, (void **)&local_this)) {
    return nullptr;
  }

  // 1-PTA_LVecBase3f GeomStore::get_vertex_positions(void) const
  PTA_LVecBase3f *return_value = new PTA_LVecBase3f(((const GeomStore*)local_this)->_vertex_positions);
  if (return_value == nullptr) {
    return PyErr_NoMemory();
  }
  return DTool_CreatePyInstance((void *)return_value, *Dtool_Ptr_PointerToArray_LVecBase3f, true, false);
}

static PyObject *Dtool_GeomStore__colors_Getter(PyObject *self, void *) {
  const GeomStore *local_this = nullptr;
  if (!Dtool_Call_ExtractThisPointer(self, Dtool_GeomStore, (void **)&local_this)) {
    return nullptr;
  }

  // 1-PTA_LVecBase4f GeomStore::get_colors(void) const
  PTA_LVecBase4f *return_value = new PTA_LVecBase4f(((const GeomStore*)local_this)->_colors);
  if (return_value == nullptr) {
    return PyErr_NoMemory();
  }
  return DTool_CreatePyInstance((void *)return_value, *Dtool_Ptr_PointerToArray_UnalignedLVecBase4f, true, false);
}

static PyObject *Dtool_GeomStore__triangle_indices_Getter(PyObject *self, void *) {
  const GeomStore *local_this = nullptr;
  if (!Dtool_Call_ExtractThisPointer(self, Dtool_GeomStore, (void **)&local_this)) {
    return nullptr;
  }

  // 1-PTA_LVecBase3i GeomStore::get_triangle_indices(void) const
  PTA_LVecBase3i *return_value = new PTA_LVecBase3i(((const GeomStore*)local_this)->_triangle_indices);
  if (return_value == nullptr) {
    return PyErr_NoMemory();
  }
  return DTool_CreatePyInstance((void *)return_value, *Dtool_Ptr_PointerToArray_LVecBase3i, true, false);
}

/**
 * Python function wrapper for:
 * GeomStore::GeomStore(void)
 * inline GeomStore::GeomStore(GeomStore const &) = default
 */
static int Dtool_Init_GeomStore(PyObject *self, PyObject *args, PyObject *kwds) {
  if (kwds != nullptr && PyDict_Size(kwds) > 0) {
#ifdef NDEBUG
    Dtool_Raise_TypeError("function takes no keyword arguments");
    return -1;
#else
    Dtool_Raise_TypeError("GeomStore() takes no keyword arguments");
    return -1;
#endif
  }
  int parameter_count = (int)PyTuple_Size(args);
  switch (parameter_count) {
  case 0:
    {
      // 1-GeomStore::GeomStore(void)
      GeomStore *return_value = new GeomStore();
      if (return_value == nullptr) {
        PyErr_NoMemory();
        return -1;
      }
      if (Dtool_CheckErrorOccurred()) {
        delete return_value;
        return -1;
      }
      return DTool_PyInit_Finalize(self, (void *)return_value, &Dtool_GeomStore, true, false);
    }
    break;
  case 1:
    {
      PyObject *arg = PyTuple_GET_ITEM(args, 0);
      // 1-inline GeomStore::GeomStore(GeomStore const &) = default
      GeomStore const *arg_this = (GeomStore *)DTOOL_Call_GetPointerThisClass(arg, Dtool_Ptr_GeomStore, 0, "GeomStore.GeomStore", true, true);
      if (arg_this != nullptr) {
        GeomStore *return_value = new GeomStore(*arg_this);
        if (return_value == nullptr) {
          PyErr_NoMemory();
          return -1;
        }
        if (Dtool_CheckErrorOccurred()) {
          delete return_value;
          return -1;
        }
        return DTool_PyInit_Finalize(self, (void *)return_value, &Dtool_GeomStore, true, false);
      }
    }
    break;
#ifndef NDEBUG
  default:
    PyErr_Format(PyExc_TypeError,
                 "GeomStore() takes 0 or 1 arguments (%d given)",
                 parameter_count);
    return -1;
#endif
  }
  if (!_PyErr_OCCURRED()) {
    Dtool_Raise_BadArgumentsError(
      "GeomStore()\n"
      "GeomStore(const GeomStore param0)\n");
  }
  return -1;
}

static void *Dtool_UpcastInterface_GeomStore(PyObject *self, Dtool_PyTypedObject *requested_type) {
  Dtool_PyTypedObject *type = DtoolInstance_TYPE(self);
  if (type != &Dtool_GeomStore) {
    printf("GeomStore ** Bad Source Type-- Requesting Conversion from %s to %s\n", Py_TYPE(self)->tp_name, requested_type->_PyType.tp_name); fflush(nullptr);
    return nullptr;
  }

  GeomStore *local_this = (GeomStore *)DtoolInstance_VOID_PTR(self);
  if (requested_type == &Dtool_GeomStore) {
    return local_this;
  }
  return nullptr;
}

static void *Dtool_DowncastInterface_GeomStore(void *from_this, Dtool_PyTypedObject *from_type) {
  if (from_this == nullptr || from_type == nullptr) {
    return nullptr;
  }
  if (from_type == Dtool_Ptr_GeomStore) {
    return from_this;
  }
  return nullptr;
}

/**
 * Python method tables for GeomStore (GeomStore)
 */
static PyMethodDef Dtool_Methods_GeomStore[] = {
  {"print_vertices", &Dtool_GeomStore_print_vertices_4, METH_NOARGS, (const char *)Dtool_GeomStore_print_vertices_4_comment},
  {"print_colors", &Dtool_GeomStore_print_colors_5, METH_NOARGS, (const char *)Dtool_GeomStore_print_colors_5_comment},
  {"print_triangles", &Dtool_GeomStore_print_triangles_6, METH_NOARGS, (const char *)Dtool_GeomStore_print_triangles_6_comment},
  {"add_vertex", (PyCFunction) &Dtool_GeomStore_add_vertex_7, METH_VARARGS | METH_KEYWORDS, (const char *)Dtool_GeomStore_add_vertex_7_comment},
  {"add_triangle", (PyCFunction) &Dtool_GeomStore_add_triangle_8, METH_VARARGS | METH_KEYWORDS, (const char *)Dtool_GeomStore_add_triangle_8_comment},
  {"subdivide_triangles", &Dtool_GeomStore_subdivide_triangles_9, METH_O, (const char *)Dtool_GeomStore_subdivide_triangles_9_comment},
  {"subdivide_triangles_distance", &Dtool_GeomStore_subdivide_triangles_distance_10, METH_O, (const char *)Dtool_GeomStore_subdivide_triangles_distance_10_comment},
  {"extend", &Dtool_GeomStore_extend_11, METH_O, (const char *)Dtool_GeomStore_extend_11_comment},
  {"__copy__", &copy_from_copy_constructor, METH_NOARGS, nullptr},
  {"__deepcopy__", &map_deepcopy_to_copy, METH_VARARGS, nullptr},
  {nullptr, nullptr, 0, nullptr}
};

//////////////////
// A wrapper function to satisfy Python's internal calling conventions.
// GeomStore slot nb_add -> operator +
//////////////////
static PyObject *Dtool_GeomStore_operator_12_nb_add(PyObject *self, PyObject *arg) {
  GeomStore *local_this = nullptr;
  DTOOL_Call_ExtractThisPointerForType(self, &Dtool_GeomStore, (void **)&local_this);
  if (local_this == nullptr) {
    Py_INCREF(Py_NotImplemented);
    return Py_NotImplemented;
  }
  if (!DtoolInstance_IS_CONST(self)) {
    {
      // -2 int GeomStore::operator +(LVecBase3f v)
      LVecBase3f *arg_this = nullptr;
      DtoolInstance_GetPointer(arg, arg_this, *Dtool_Ptr_LVecBase3f);
      if (arg_this != nullptr) {
        int return_value = (*local_this).operator +(*arg_this);
#ifndef NDEBUG
        Notify *notify = Notify::ptr();
        if (UNLIKELY(notify->has_assert_failed())) {
          return Dtool_Raise_AssertionError();
        }
#endif
        return Dtool_WrapValue(return_value);
      }
    }

    {
      // -2 int GeomStore::operator +(float v)
      if (PyNumber_Check(arg)) {
        int return_value = (*local_this).operator +((float)PyFloat_AsDouble(arg));
#ifndef NDEBUG
        Notify *notify = Notify::ptr();
        if (UNLIKELY(notify->has_assert_failed())) {
          return Dtool_Raise_AssertionError();
        }
#endif
        return Dtool_WrapValue(return_value);
      }
    }

    {
      // -2 int GeomStore::operator +(LVecBase3f v)
      LVecBase3f arg_local;
      LVecBase3f *arg_this = Dtool_Coerce_LVecBase3f(arg, arg_local);
      if ((arg_this != nullptr)) {
        int return_value = (*local_this).operator +(*arg_this);
#ifndef NDEBUG
        Notify *notify = Notify::ptr();
        if (UNLIKELY(notify->has_assert_failed())) {
          return Dtool_Raise_AssertionError();
        }
#endif
        return Dtool_WrapValue(return_value);
      }
    }

    // No coercion possible: int GeomStore::operator +(float v)
  } else {
#ifdef NDEBUG
    return Dtool_Raise_TypeError("non-const method called on const object");
#else
    return Dtool_Raise_TypeError("Cannot call GeomStore.__add__() on a const object.");
#endif
  }
  Py_INCREF(Py_NotImplemented);
  return Py_NotImplemented;
}

//////////////////
// A wrapper function to satisfy Python's internal calling conventions.
// GeomStore slot nb_divide -> operator /
//////////////////
static PyObject *Dtool_GeomStore_operator_15_nb_divide(PyObject *self, PyObject *arg) {
  GeomStore *local_this = nullptr;
  DTOOL_Call_ExtractThisPointerForType(self, &Dtool_GeomStore, (void **)&local_this);
  if (local_this == nullptr) {
    Py_INCREF(Py_NotImplemented);
    return Py_NotImplemented;
  }
  if (!DtoolInstance_IS_CONST(self)) {
    // 1-int GeomStore::operator /(float v)
    if (PyNumber_Check(arg)) {
      int return_value = (*local_this).operator /((float)PyFloat_AsDouble(arg));
#ifndef NDEBUG
      Notify *notify = Notify::ptr();
      if (UNLIKELY(notify->has_assert_failed())) {
        return Dtool_Raise_AssertionError();
      }
#endif
      return Dtool_WrapValue(return_value);
    }
  } else {
#ifdef NDEBUG
    return Dtool_Raise_TypeError("non-const method called on const object");
#else
    return Dtool_Raise_TypeError("Cannot call GeomStore.__div__() on a const object.");
#endif
  }
  Py_INCREF(Py_NotImplemented);
  return Py_NotImplemented;
}

//////////////////
// A wrapper function to satisfy Python's internal calling conventions.
// GeomStore slot nb_multiply -> operator *
//////////////////
static PyObject *Dtool_GeomStore_operator_14_nb_multiply(PyObject *self, PyObject *arg) {
  GeomStore *local_this = nullptr;
  DTOOL_Call_ExtractThisPointerForType(self, &Dtool_GeomStore, (void **)&local_this);
  if (local_this == nullptr) {
    Py_INCREF(Py_NotImplemented);
    return Py_NotImplemented;
  }
  if (!DtoolInstance_IS_CONST(self)) {
    // 1-int GeomStore::operator *(float v)
    if (PyNumber_Check(arg)) {
      int return_value = (*local_this).operator *((float)PyFloat_AsDouble(arg));
#ifndef NDEBUG
      Notify *notify = Notify::ptr();
      if (UNLIKELY(notify->has_assert_failed())) {
        return Dtool_Raise_AssertionError();
      }
#endif
      return Dtool_WrapValue(return_value);
    }
  } else {
#ifdef NDEBUG
    return Dtool_Raise_TypeError("non-const method called on const object");
#else
    return Dtool_Raise_TypeError("Cannot call GeomStore.__mul__() on a const object.");
#endif
  }
  Py_INCREF(Py_NotImplemented);
  return Py_NotImplemented;
}

//////////////////
// A wrapper function to satisfy Python's internal calling conventions.
// GeomStore slot nb_subtract -> operator -
//////////////////
static PyObject *Dtool_GeomStore_operator_13_nb_subtract(PyObject *self, PyObject *arg) {
  GeomStore *local_this = nullptr;
  DTOOL_Call_ExtractThisPointerForType(self, &Dtool_GeomStore, (void **)&local_this);
  if (local_this == nullptr) {
    Py_INCREF(Py_NotImplemented);
    return Py_NotImplemented;
  }
  if (!DtoolInstance_IS_CONST(self)) {
    {
      // -2 int GeomStore::operator -(LVecBase3f v)
      LVecBase3f *arg_this = nullptr;
      DtoolInstance_GetPointer(arg, arg_this, *Dtool_Ptr_LVecBase3f);
      if (arg_this != nullptr) {
        int return_value = (*local_this).operator -(*arg_this);
#ifndef NDEBUG
        Notify *notify = Notify::ptr();
        if (UNLIKELY(notify->has_assert_failed())) {
          return Dtool_Raise_AssertionError();
        }
#endif
        return Dtool_WrapValue(return_value);
      }
    }

    {
      // -2 int GeomStore::operator -(float v)
      if (PyNumber_Check(arg)) {
        int return_value = (*local_this).operator -((float)PyFloat_AsDouble(arg));
#ifndef NDEBUG
        Notify *notify = Notify::ptr();
        if (UNLIKELY(notify->has_assert_failed())) {
          return Dtool_Raise_AssertionError();
        }
#endif
        return Dtool_WrapValue(return_value);
      }
    }

    {
      // -2 int GeomStore::operator -(LVecBase3f v)
      LVecBase3f arg_local;
      LVecBase3f *arg_this = Dtool_Coerce_LVecBase3f(arg, arg_local);
      if ((arg_this != nullptr)) {
        int return_value = (*local_this).operator -(*arg_this);
#ifndef NDEBUG
        Notify *notify = Notify::ptr();
        if (UNLIKELY(notify->has_assert_failed())) {
          return Dtool_Raise_AssertionError();
        }
#endif
        return Dtool_WrapValue(return_value);
      }
    }

    // No coercion possible: int GeomStore::operator -(float v)
  } else {
#ifdef NDEBUG
    return Dtool_Raise_TypeError("non-const method called on const object");
#else
    return Dtool_Raise_TypeError("Cannot call GeomStore.__sub__() on a const object.");
#endif
  }
  Py_INCREF(Py_NotImplemented);
  return Py_NotImplemented;
}

#if PY_VERSION_HEX >= 0x3000000
//////////////////
// A wrapper function to satisfy Python's internal calling conventions.
// GeomStore slot nb_true_divide -> operator /
//////////////////
static PyObject *Dtool_GeomStore_operator_15_nb_true_divide(PyObject *self, PyObject *arg) {
  GeomStore *local_this = nullptr;
  DTOOL_Call_ExtractThisPointerForType(self, &Dtool_GeomStore, (void **)&local_this);
  if (local_this == nullptr) {
    Py_INCREF(Py_NotImplemented);
    return Py_NotImplemented;
  }
  if (!DtoolInstance_IS_CONST(self)) {
    // 1-int GeomStore::operator /(float v)
    if (PyNumber_Check(arg)) {
      int return_value = (*local_this).operator /((float)PyFloat_AsDouble(arg));
#ifndef NDEBUG
      Notify *notify = Notify::ptr();
      if (UNLIKELY(notify->has_assert_failed())) {
        return Dtool_Raise_AssertionError();
      }
#endif
      return Dtool_WrapValue(return_value);
    }
  } else {
#ifdef NDEBUG
    return Dtool_Raise_TypeError("non-const method called on const object");
#else
    return Dtool_Raise_TypeError("Cannot call GeomStore.__div__() on a const object.");
#endif
  }
  Py_INCREF(Py_NotImplemented);
  return Py_NotImplemented;
}

#endif  // PY_VERSION_HEX >= 0x3000000
static PyGetSetDef Dtool_Properties_GeomStore[] = {
  {(char *)"_vertex_positions", &Dtool_GeomStore__vertex_positions_Getter, nullptr, nullptr, nullptr},
  {(char *)"_colors", &Dtool_GeomStore__colors_Getter, nullptr, nullptr, nullptr},
  {(char *)"_triangle_indices", &Dtool_GeomStore__triangle_indices_Getter, nullptr, nullptr, nullptr},
  {nullptr},
};

static PyNumberMethods Dtool_NumberMethods_GeomStore = {
  &Dtool_GeomStore_operator_12_nb_add,
  &Dtool_GeomStore_operator_13_nb_subtract,
  &Dtool_GeomStore_operator_14_nb_multiply,
#if PY_MAJOR_VERSION < 3
  &Dtool_GeomStore_operator_15_nb_divide,
#endif
  nullptr,
  nullptr,
  nullptr,
  nullptr,
  nullptr,
  nullptr,
  nullptr,
  nullptr,
  nullptr,
  nullptr,
  nullptr,
  nullptr,
  nullptr,
#if PY_MAJOR_VERSION < 3
  nullptr,
#endif
  nullptr,
  nullptr, // nb_long
  nullptr,
#if PY_MAJOR_VERSION < 3
  nullptr,
  nullptr,
#endif
  nullptr,
  nullptr,
  nullptr,
#if PY_MAJOR_VERSION < 3
  nullptr,
#endif
  nullptr,
  nullptr,
  nullptr,
  nullptr,
  nullptr,
  nullptr,
  nullptr,
  nullptr,
#if PY_VERSION_HEX >= 0x3000000
  &Dtool_GeomStore_operator_15_nb_true_divide,
#else
  nullptr,
#endif
  nullptr,
  nullptr,
#if PY_VERSION_HEX >= 0x02050000
  nullptr,
#endif
#if PY_VERSION_HEX >= 0x03050000
  nullptr,
  nullptr,
#endif
};

struct Dtool_PyTypedObject Dtool_GeomStore = {
  {
    PyVarObject_HEAD_INIT(nullptr, 0)
    "geomstore.GeomStore",
    sizeof(Dtool_PyInstDef),
    0, // tp_itemsize
    &Dtool_FreeInstance_GeomStore,
    nullptr,
    nullptr,
    nullptr,
#if PY_VERSION_HEX >= 0x03050000
    nullptr, // tp_as_async
#elif PY_MAJOR_VERSION >= 3
    nullptr, // tp_reserved
#else
    nullptr, // tp_compare
#endif
    nullptr,
    &Dtool_NumberMethods_GeomStore,
    nullptr, // tp_as_sequence
    nullptr, // tp_as_mapping
    nullptr, // tp_hash
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr, // tp_as_buffer
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE | Py_TPFLAGS_CHECKTYPES,
    nullptr, // tp_doc
    nullptr, // tp_traverse
    nullptr, // tp_clear
    nullptr, // tp_richcompare
    0, // tp_weaklistoffset
    nullptr,
    nullptr,
    Dtool_Methods_GeomStore,
    nullptr, // tp_members
    Dtool_Properties_GeomStore,
    nullptr, // tp_base
    nullptr, // tp_dict
    nullptr,
    nullptr,
    0, // tp_dictoffset
    Dtool_Init_GeomStore,
    PyType_GenericAlloc,
    Dtool_new_GeomStore,
    PyObject_Del,
    nullptr, // tp_is_gc
    nullptr, // tp_bases
    nullptr, // tp_mro
    nullptr, // tp_cache
    nullptr, // tp_subclasses
    nullptr, // tp_weaklist
    nullptr, // tp_del
#if PY_VERSION_HEX >= 0x02060000
    0, // tp_version_tag
#endif
#if PY_VERSION_HEX >= 0x03040000
    nullptr, // tp_finalize
#endif
  },
  TypeHandle::none(),
  Dtool_PyModuleClassInit_GeomStore,
  Dtool_UpcastInterface_GeomStore,
  Dtool_DowncastInterface_GeomStore,
  nullptr,
  nullptr,
};

static void Dtool_PyModuleClassInit_GeomStore(PyObject *module) {
  (void) module; // Unused
  static bool initdone = false;
  if (!initdone) {
    initdone = true;
    // Dependent objects
    Dtool_GeomStore._PyType.tp_base = (PyTypeObject *)Dtool_GetSuperBase();
    PyObject *dict = PyDict_New();
    Dtool_GeomStore._PyType.tp_dict = dict;
    PyDict_SetItemString(dict, "DtoolClassDict", dict);
    if (PyType_Ready((PyTypeObject *)&Dtool_GeomStore) < 0) {
      Dtool_Raise_TypeError("PyType_Ready(GeomStore)");
      return;
    }
    Py_INCREF((PyTypeObject *)&Dtool_GeomStore);
  }
}


/**
 * Module Object Linker ..
 */
void Dtool_geomstore_RegisterTypes() {
  TypeRegistry *registry = TypeRegistry::ptr();
  nassertv(registry != nullptr);
}

void Dtool_geomstore_BuildInstants(PyObject *module) {
  (void) module;
  // GeomStore
  Dtool_PyModuleClassInit_GeomStore(module);
  PyModule_AddObject(module, "GeomStore", (PyObject *)&Dtool_GeomStore);
}

static PyMethodDef python_simple_funcs[] = {
  // Support Function For Dtool_types ... for now in each module ??
  {"Dtool_BorrowThisReference", &Dtool_BorrowThisReference, METH_VARARGS, "Used to borrow 'this' pointer (to, from)\nAssumes no ownership."},
  {nullptr, nullptr, 0, nullptr}
};

#ifdef LINK_ALL_STATIC
extern const struct LibraryDef geomstore_moddef = {python_simple_funcs, exports, nullptr};
#else
extern const struct LibraryDef geomstore_moddef = {python_simple_funcs, exports, imports};
#endif
static InterrogateModuleDef _in_module_def = {
  1546885810,  /* file_identifier */
  "geomstore",  /* library_name */
  "Kshb",  /* library_hash_name */
  "geomstore",  /* module_name */
  "interrogate.in",  /* database_filename */
  nullptr,  /* unique_names */
  0,  /* num_unique_names */
  nullptr,  /* fptrs */
  0,  /* num_fptrs */
  1,  /* first_index */
  54  /* next_index */
};

Configure(_in_configure_geomstore);
ConfigureFn(_in_configure_geomstore) {
  interrogate_request_module(&_in_module_def);
}

