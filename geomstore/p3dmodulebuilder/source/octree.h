/**
 * Rudimentary Octree implementation to accelerate 
 * Triangle-Triangle-Intersection testing.
 * 
 * @file octree.h
 * @author Tiziano Bettio
 * @date 2019-01-29
 */

/* interrogate doesn't need to know about Octree */
#ifndef CPPPARSER

#ifndef OCTREE_H
#define OCTREE_H

#include <vector>
#include <memory>
#include <iostream>

#include "aabb.h"


class Octree {
  public:
    Octree(AABB aabb, int max_depth = 6, bool root = true);
    ~Octree();
    Octree(const Octree& oct);
    bool add_id(const AABB& aabb, unsigned int id);
    bool get_ids(const AABB& aabb, std::vector<unsigned int>& rvec);
    /* xyz - p=positive n=negative */
  private:
    std::vector<std::unique_ptr<Octree>> _children;
    AABB _aabb;
    int _max_depth;
    std::vector<unsigned int> _ids;
    bool _is_root;
};

#endif  /* OCTREE_H */
#endif  /* CPPPARSER */