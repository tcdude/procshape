/**
 * @file csg.h
 * @author Tiziano Bettio
 * @date 2019-01-20
 */

/* interrogate doesn't need to know about CSG */
#ifndef CPPPARSER


#ifndef CSG_H
#define CSG_H

#include <vector>

#include "luse.h"

#include "aabb.h"
#include "octree.h"
#include "mesh.h"

struct Triangle {
  LVecBase3f V0, V1, V2;
};

struct Ray {
  LVecBase3f P0, P1;
};

typedef std::vector<MyMesh::Point> PointVec;
typedef std::vector<MyMesh::Point>::size_type vec_size_type;
typedef std::vector<std::pair<vec_size_type, vec_size_type>> EdgeVec;

struct IsectEdges {
  PointVec pts;
  EdgeVec edges;
};

int intersect3d_ray_triangle(Ray ray, Triangle tri, LVecBase3f& isect_p);
void csg_union(Mesh& a, Mesh& b, Mesh& out);
void csg_difference(Mesh& a, Mesh& b, Mesh& out);
void csg_intersect(Mesh& a, Mesh& b, Mesh& out);
bool verify_special_cases(
    Mesh& a, 
    Mesh& b, 
    std::vector<Mesh>& res_a, 
    std::vector<Mesh>& res_b);
void process_mesh_intersection(Mesh& a, Mesh& b, PointVec& p, EdgeVec& e);
void find_intersecting_segment(
    float (&v)[3][3], 
    float (&u)[3][3],
    MyMesh::Point (&edge)[2]);

#endif
#endif
