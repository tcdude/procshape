/**
 * Representation of arbitrary geometry, stored in OpenMesh Kernel and
 * accessible through memoryview in Python. Provides subdivision, mesh
 * decimation and smoothing.
 */

/* interrogate doesn't need to know about OpenMesh */
#ifndef CPPPARSER


#ifndef MESH_H
#define MESH_H

#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>
#include "pta_LVecBase3.h"
#include "pta_LVecBase4.h"

class GeomStore;

typedef OpenMesh::TriMesh_ArrayKernelT<> MyMesh;

struct AABB {
  LVecBase3f box;
  LPoint3f origin;
  bool calculated = false;
};

class Mesh {
  public:
    Mesh();
    ~Mesh();
    void subdivision(GeomStore* g, int c = 1);
    void subdivision_length(GeomStore* g, double l = 2.0);
    void decimate(GeomStore* g, float max_err);
    void regen_aabb();
  private:
    AABB aabb;
    void fill_mesh(GeomStore* g);
    void fill_geomstore(GeomStore* g);
    MyMesh _my_mesh;
    bool _filled = false;
};
#endif

#endif