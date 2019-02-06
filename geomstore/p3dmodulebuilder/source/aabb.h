 /**
 * @file aabb.h
 * @author Tiziano Bettio
 * @date 2019-02-02
 */


/* interrogate doesn't need to know about AABB */
#ifndef CPPPARSER

#ifndef AABB_H
#define AABB_H

#include "luse.h"
#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>

typedef OpenMesh::TriMesh_ArrayKernelT<> MyMesh;

enum Octant {ppp, npp, pnp, nnp, ppn, npn, pnn, nnn};
const LVecBase3f oct_modifiers[8] = {
  LVecBase3f(1.0f, 1.0f, 1.0f),
  LVecBase3f(-1.0f, 1.0f, 1.0f),
  LVecBase3f(1.0f, -1.0f, 1.0f),
  LVecBase3f(-1.0f, -1.0f, 1.0f),
  LVecBase3f(1.0f, 1.0f, -1.0f),
  LVecBase3f(-1.0f, 1.0f, -1.0f),
  LVecBase3f(1.0f, -1.0f, -1.0f),
  LVecBase3f(-1.0f, -1.0f, -1.0f)
};

struct AABB {
  LVecBase3f box;
  LPoint3f origin;
  bool calculated = false;
  AABB aabb_intersection(AABB& other);
  bool inside(LVecBase3f p);
  bool inside(LPoint3f p);
  bool inside(MyMesh::Point p);
  bool inside(const AABB& aabb, bool completely = true);
  bool get_octant(const AABB& aabb, Octant& oct);
};

#endif
#endif
