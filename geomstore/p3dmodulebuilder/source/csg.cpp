/**
 * Constructive Solid Geometry implementation.
 * This code makes use of a ray-triangle intersection algorithm
 * from softSurfer / Dan Sunday as stated below. Minor cosmetic changes
 * to fit into this code base have been performed. 
 * 
 * @file csg.cpp
 * @author Tiziano Bettio
 * @date 2019-01-20
 */

/* interrogate doesn't need to know about CSG */
#ifndef CPPPARSER
#include "csg.h"

#include <tuple>
#include "tritri.h"
#include "config_module.h"
#include "common.h"

// Copyright 2001 softSurfer, 2012 Dan Sunday
// This code may be freely used and modified for any purpose
// providing that this copyright notice is included with it.
// SoftSurfer makes no warranty for this code, and cannot be held
// liable for any real or imagined damage resulting from its use.
// Users of this code must verify correctness for their application.
 

// Assume that classes are already given for the objects:
//    Point and Vector with
//        coordinates {float x, y, z;}
//        operators for:
//            == to test  equality
//            != to test  inequality
//            (Vector)0 =  (0,0,0)         (null vector)
//            Point   = Point ± Vector
//            Vector =  Point - Point
//            Vector =  Scalar * Vector    (scalar product)
//            Vector =  Vector * Vector    (cross product)
//    Line and Ray and Segment with defining  points {Point P0, P1;}
//        (a Line is infinite, Rays and  Segments start at P0)
//        (a Ray extends beyond P1, but a  Segment ends at P1)
//    Plane with a point and a normal {Point V0; Vector  n;}
//    Triangle with defining vertices {Point V0, V1, V2;}
//    Polyline and Polygon with n vertices {int n;  Point *V;}
//        (a Polygon has V[n]=V[0])
//===================================================================
 

#define SMALL_NUM   0.00000001 // anything that avoids division overflow


// intersect3D_RayTriangle(): find the 3D intersection of a ray with a triangle
//    Input:  a ray R, and a triangle T
//    Output: *I = intersection point (when it exists)
//    Return: -1 = triangle is degenerate (a segment or point)
//             0 =  disjoint (no intersect)
//             1 =  intersect in unique point I1
//             2 =  are in the same plane
int intersect3d_ray_triangle(Ray ray, Triangle tri, LVecBase3f& isect_p )
{
  LVecBase3f    u, v, n;              // triangle vectors
  LVecBase3f    dir, w0, w;           // ray vectors
  float     r, a, b;              // params to calc ray-plane intersect

  // get triangle edge vectors and plane normal
  u = tri.V1 - tri.V0;
  v = tri.V2 - tri.V0;
  n = u.cross(v);              // cross product
  if (n == (LVecBase3f)0)             // triangle is degenerate
      return -1;                  // do not deal with this case

  dir = ray.P1 - ray.P0;              // ray direction vector
  w0 = ray.P0 - tri.V0;
  a = -dot(n, w0);
  b = dot(n, dir);
  if (fabs(b) < SMALL_NUM) {     // ray is  parallel to triangle plane
      if (a == 0)                 // ray lies in triangle plane
          return 2;
      else return 0;              // ray disjoint from plane
  }

  // get intersect point of ray with triangle plane
  r = a / b;
  if (r < 0.0)                    // ray goes away from triangle
      return 0;                   // => no intersect
  // for a segment, also test if (r > 1.0) => no intersect

  isect_p = ray.P0 + r * dir;            // intersect point of ray and plane

  // is isect_p inside tri?
  float    uu, uv, vv, wu, wv, D;
  uu = dot(u, u);
  uv = dot(u, v);
  vv = dot(v, v);
  w = isect_p - tri.V0;
  wu = dot(w, u);
  wv = dot(w, v);
  D = uv * uv - uu * vv;

  // get and test parametric coords
  float s, t;
  s = (uv * wv - vv * wu) / D;
  if (s < 0.0 || s > 1.0)         // isect_p is outside tri
      return 0;
  t = (uv * wu - uu * wv) / D;
  if (t < 0.0 || (s + t) > 1.0)  // isect_p is outside tri
      return 0;

  return 1;                       // isect_p is in tri
}
 

/**
 * Returns the union of Mesh `a` + `b`.
 */
void csg_union(Mesh& a, Mesh& b, Mesh& out) {
  std::vector<Mesh> res_a, res_b;
  if (verify_special_cases(a, b, res_a, res_b)) {
    for (auto it : res_a) {
      if (it._my_mesh.property(it._outside)) {
        out.extend(it);
      }
    }
    for (auto it : res_b) {
      if (it._my_mesh.property(it._outside)) {
        out.extend(it);
      }
    }
    return;
  }
}


/**
 * Returns the difference of Mesh `a` - `b`. The order matters, as
 * `a` - `b` produces a different different than `b` - `a`!
 */
void csg_difference(Mesh& a, Mesh& b, Mesh& out) {
  std::vector<Mesh> res_a, res_b;
  if (verify_special_cases(a, b, res_a, res_b)) {
    for (auto it : res_a) {
      if (it._my_mesh.property(it._outside)) {
        out.extend(it);
      }
    }
    for (auto it : res_b) {
      if (!it._my_mesh.property(it._outside)) {
        out.extend(it, true);
      }
    }
    return;
  }
}


/**
 * Returns the intersection of Mesh `a` and `b`.
 */
void csg_intersect(Mesh& a, Mesh& b, Mesh& out) {
  std::vector<Mesh> res_a, res_b;
  if (verify_special_cases(a, b, res_a, res_b)) {
    for (auto it : res_a) {
      if (!it._my_mesh.property(it._outside)) {
        out.extend(it);
      }
    }
    for (auto it : res_b) {
      if (!it._my_mesh.property(it._outside)) {
        out.extend(it);
      }
    }
    return;
  }
}


/**
 * Returns true, if a special case was identified, otherwise false.
 */

bool verify_special_cases(
    Mesh& a, 
    Mesh& b, 
    std::vector<Mesh>& res_a, 
    std::vector<Mesh>& res_b) {
  /* Test cases where one AABB is completely inside the other */
  /* Is b completly inside a? */
  if (a.aabb.inside(b.aabb.origin + b.aabb.box) 
      && a.aabb.inside(b.aabb.origin - b.aabb.box)) {
  /* No points of a inside AABB of b? */
    if (!a.has_points_inside(b)) { 
      Mesh t = a;
      Mesh o = b;
      o._my_mesh.property(o._outside) = false;
      res_a.push_back(t);
      res_b.push_back(o);
      return true;
    }
  }

  /* Is a completly inside b? */
  if (b.aabb.inside(a.aabb.origin + a.aabb.box) 
      && b.aabb.inside(a.aabb.origin - a.aabb.box)) {
  /* No points of b inside AABB of a? */
    if (!b.has_points_inside(a)) { 
      Mesh t = a;
      Mesh o = b;
      t._my_mesh.property(t._outside) = false;
      res_a.push_back(t);
      res_b.push_back(o);
      return true;
    }
  }


  AABB intersection = a.aabb.aabb_intersection(b.aabb);
  /* No intersection of the meshes */
  if (!intersection.calculated) {  
    Mesh t = a;
    Mesh o = b;
    res_a.push_back(t);
    res_b.push_back(o);
    return true;
  }
  return false;
}


/**
 * Searches for intersecting triangles and adds the resulting edges
 * to `e` and the intersecting triangles.
 */

void process_mesh_intersection(Mesh& a, Mesh& b, PointVec& p, EdgeVec& e) {
  Octree oct_a = a.get_octree();
  Octree oct_b = b.get_octree();
  MyMesh& mma = a._my_mesh;
  MyMesh& mmb = b._my_mesh;
  MyMesh::FaceIter f_it, f_end = mma.faces_end();
  IsectEdges isect_edges;
  for (f_it = mma.faces_begin(); f_it != f_end; ++f_it) {
    float v[3][3], u[3][3];
    MyMesh::FaceVertexCCWIter fv_it = mma.fv_ccwiter(*f_it);
    int i = 0;
    for (fv_it; fv_it.is_valid(); ++fv_it) {
      v[i][0] = mma.point(*fv_it)[0];
      v[i][1] = mma.point(*fv_it)[1];
      v[i++][2] = mma.point(*fv_it)[2];
    } 
    for (auto it : oct_b.get_ids(mma.property(a._tri_aabb, *f_it))) {
      MyMesh::FaceHandle bfh = mmb.face_handle(it);
      MyMesh::FaceVertexCCWIter bfv_it = mmb.fv_ccwiter(bfh);
      int j = 0;
      for (bfv_it; bfv_it.is_valid(); ++bfv_it) {
        u[j][0] = mmb.point(*bfv_it)[0];
        u[j][1] = mmb.point(*bfv_it)[1];
        u[j++][2] = mmb.point(*bfv_it)[2];
      }
      if (tri_tri_intersect(v[0], v[1], v[2], u[0], u[1], u[2])) {
        TriIntersection tia, tib;
        MyMesh::Point edge[2];
        find_intersecting_segment(v, u, edge);
        std::pair<bool, int> res0 = find_in_vec(p, edge[0]);
        std::pair<bool, int> res1 = find_in_vec(p, edge[1]);
        std::pair<unsigned int, unsigned int> tmp_edge;
        if (res0.first) {
          tmp_edge.first = res0.second;
        }
        else {
          tmp_edge.first = p.size();
          p.push_back(edge[0]);
        }
        if (res1.first) {
          tmp_edge.second = res1.second;
        }
        else {
          tmp_edge.second = p.size();
          p.push_back(edge[1]);
        }
        e.push_back(tmp_edge);

        tia.other_id = it;
        tib.other_id = f_it->idx();
        tia.edge[0] = edge[0];
        tia.edge[1] = edge[1];
        tib.edge[0] = edge[0];
        tib.edge[1] = edge[1];
        mma.property(a._tri_intersection, *f_it).push_back(tia);
        mmb.property(b._tri_intersection, bfh).push_back(tib);
      }
    }
  }
}


/**
 * Finds the two intersection points of two intersecting triangles.
 */

void find_intersecting_segment(
    float (&v)[3][3],
    float (&u)[3][3],
    MyMesh::Point (&edge)[2]) {

  Triangle t[2];
  t[0].V0 = v[0];
  t[0].V1 = v[1];
  t[0].V2 = v[2];

  t[1].V0 = u[0];
  t[1].V1 = u[1];
  t[1].V2 = u[2];

  Ray r[2][3];
  r[0][0].P0 = t[0].V0;
  r[0][0].P1 = t[0].V1;
  r[0][1].P0 = t[0].V1;
  r[0][1].P1 = t[0].V2;
  r[0][2].P0 = t[0].V2;
  r[0][2].P1 = t[0].V0;

  r[1][0].P0 = t[1].V0;
  r[1][0].P1 = t[1].V1;
  r[1][1].P0 = t[1].V1;
  r[1][1].P1 = t[1].V2;
  r[1][2].P0 = t[1].V2;
  r[1][2].P1 = t[1].V0;

  int ei = 0;
  for (int i = 0; i < 2; ++i) {
    for (int j = 0; j < 3; ++j) {
      LVecBase3f isect;
      int res = intersect3d_ray_triangle(r[i][j], t[(i + 1) % 2], isect);
      if (res == 1) {
        edge[ei][0] = isect[0];
        edge[ei][1] = isect[1];
        edge[ei][2] = isect[2];
        if (ei == 1) {
          return;
        }
        ++ei;
      }
    }
  }

  geomstore_cat.error() << "Did not find two intersecting points!!" << std::endl;
}

#endif
