/**
 * Representation of arbitrary geometry, stored in OpenMesh Kernel and
 * accessible through memoryview in Python. Provides subdivision, mesh
 * decimation and smoothing.
 */

/* interrogate doesn't need to know about OpenMesh */
#ifndef CPPPARSER


#ifndef MESH_H
#define MESH_H

#include <iostream>

#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>
typedef OpenMesh::TriMesh_ArrayKernelT<> MyMesh;


class Mesh {
  public:
    Mesh();
    ~Mesh();
    MyMesh _my_mesh;
};
#endif

#endif