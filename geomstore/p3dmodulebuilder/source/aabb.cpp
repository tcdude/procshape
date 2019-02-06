 /**
 * @file aabb.cpp
 * @author Tiziano Bettio
 * @date 2019-02-02
 */

/* interrogate doesn't need to know about AABB */
#ifndef CPPPARSER

#include "aabb.h"


/**
 * Returns a new AABB that represents the intersection of two AABBs.
 * If there is no intersection, an empty AABB struct is returned instead.
 */
AABB AABB::
aabb_intersection(AABB& other) {
    LVecBase3f this_max = origin + box;
    LVecBase3f this_min = origin - box;

    LVecBase3f other_max = other.origin + other.box;
    LVecBase3f other_min = other.origin - other.box;
    
    LVecBase3f new_max = LVecBase3f(
      std::min(this_max[0], other_max[0]),
      std::min(this_max[1], other_max[1]),
      std::min(this_max[2], other_max[2])
    );
    
    LVecBase3f new_min = LVecBase3f(
      std::max(this_min[0], other_min[0]),
      std::max(this_min[1], other_min[1]),
      std::max(this_min[2], other_min[2])
    );
    
    for (int i = 0; i < 3; i++) {
      if (new_min[i] >= new_max[i]) {
        return AABB();
      }
    }

    AABB isect;

    isect.box = (new_max - new_min) * 0.5f;
    isect.origin = new_min + isect.box;
    isect.box *= 1.001;
    isect.calculated = true;
    return isect;
  }


/**
 * Returns true if `p` lies inside of the AABB.
 */
bool AABB::
inside(LVecBase3f p) {
  bool r;
  r = (std::abs(origin[0] - p[0]) <= box[0] 
       && std::abs(origin[1] - p[1]) <= box[1]
       && std::abs(origin[2] - p[2]) <= box[2]) ? true : false;
  return r;
}


/**
 * Returns true if `p` lies inside of the AABB.
 */
bool AABB::
inside(LPoint3f p) {
  bool r;
  r = (std::abs(origin[0] - p[0]) <= box[0] 
       && std::abs(origin[1] - p[1]) <= box[1]
       && std::abs(origin[2] - p[2]) <= box[2]) ? true : false;
  return r;
}


/**
 * Returns true if `p` lies inside of the AABB.
 */
bool AABB::
inside(MyMesh::Point p) {
  bool r;
  r = (std::abs(origin[0] - p[0]) <= box[0] 
       && std::abs(origin[1] - p[1]) <= box[1]
       && std::abs(origin[2] - p[2]) <= box[2]) ? true : false;
  return r;
}


/**
 * Returns true if `aabb` lies completely inside of the AABB.
 */
bool AABB::
inside(const AABB& aabb, bool completely) {
  if (completely) {
    if (inside(aabb.origin + aabb.box) && inside(aabb.origin - aabb.box)) {
      return true;
    }
  }
  else {
    if (inside(aabb.origin + aabb.box) || inside(aabb.origin - aabb.box)) {
      return true;
    }
  }
  return false;
}


/**
 * Returns true if an octant has been identified, otherwise false (multiple 
 * or no octants match!!).
 */
bool AABB::
get_octant(const AABB& aabb, Octant& oct) {
  if (!inside(aabb)) {
    return false;
  }
  AABB tmp;
  tmp.box = box * 0.5f;
  for (int i = 0; i < 8; i++) {
    tmp.origin = {
      origin[0] + tmp.box[0] * oct_modifiers[i][0],
      origin[1] + tmp.box[1] * oct_modifiers[i][1],
      origin[2] + tmp.box[2] * oct_modifiers[i][2]
    };
    if (inside(tmp)) {
      oct = Octant(i);
      return true;
    };
  }
  return false;
}

#endif
