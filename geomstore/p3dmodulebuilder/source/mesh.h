/**
 * Representation of arbitrary geometry, stored in OpenMesh Kernel and
 * accessible through memoryview in Python. Provides subdivision, mesh
 * decimation and smoothing.
 */

/* interrogate doesn't need to know about OpenMesh */
#ifndef CPPPARSER


#ifndef MESH_H
#define MESH_H
#include <vector>
#include <algorithm>
#include <memory>

#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>
#include "pta_LVecBase3.h"
#include "pta_LVecBase4.h"

#include "aabb.h"
#include "octree.h"

class GeomStore;

typedef OpenMesh::TriMesh_ArrayKernelT<> MyMesh;
typedef OpenMesh::Vec3f Vec3f;
typedef OpenMesh::Vec2f Vec2f;

struct TriIntersection {
  unsigned int other_id;
  MyMesh::Point edge[2];
};


void project_polygon(std::vector<Vec3f>& poly3, std::vector<Vec2f>& poly2);


class Mesh {
  public:
    Mesh(bool outside = true);
    ~Mesh();
    void subdivision(GeomStore* g, int c = 1);
    void subdivision_length(GeomStore* g, double l = 2.0);
    void decimate(GeomStore* g, float max_err);
    void extend(Mesh& other, bool inv_winding = false);
    void mesh_union(Mesh& other);
    void mesh_difference(Mesh& other);
    void mesh_intersect(Mesh& other);
    void generate_sub_surfaces(Mesh& other, 
                               std::vector<Mesh>& outside,
                               std::vector<Mesh>& inside);
    void regen_aabb();
    Octree get_octree();
    OpenMesh::HPropHandleT<bool> _processed;
    OpenMesh::MPropHandleT<bool> _outside;
    OpenMesh::FPropHandleT<AABB> _tri_aabb;
    OpenMesh::FPropHandleT<std::vector<TriIntersection>> _tri_intersection;
    AABB aabb;
    bool has_points_inside(Mesh& other);
    MyMesh _my_mesh;
  private:
    void fill_octree();
    AABB face_aabb(const MyMesh::FaceHandle& fh);
    void replace(Mesh& other);
    void fill_mesh(GeomStore* g);
    void fill_geomstore(GeomStore* g);
    std::shared_ptr<Octree> _octree;
    bool _filled = false;
    bool _dirty = true;
};
#endif

#endif