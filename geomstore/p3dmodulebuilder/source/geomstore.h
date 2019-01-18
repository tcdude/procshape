/**
 * Representation of arbitrary geometry, stored in OpenMesh Kernel and
 * accessible through memoryview in Python. Provides subdivision, mesh
 * decimation smoothing
 */


#ifndef GEOMSTORE_H
#define GEOMSTORE_H

#include <iostream>

#include "pandabase.h"
#include "luse.h"
#include "pta_LVecBase3.h"
#include "pta_LVecBase4.h"
#include "pta_int.h"
#include "pnotify.h"
#include "geomVertexData.h"
#include "geomVertexWriter.h"
#include "geom.h"
#include "geomTriangles.h"
#include "geomNode.h"
#include "Python.h"

#ifndef CPPPARSER
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>
typedef OpenMesh::TriMesh_ArrayKernelT<> MyMesh;
#endif

class GeomStore {
  PUBLISHED:
    GeomStore();
    ~GeomStore();
  private:
#ifndef CPPPARSER
    MyMesh _my_mesh;
#endif
};

#endif