/**
 * Rudimentary Octree implementation to accelerate 
 * Triangle-Triangle-Intersection testing.
 * 
 * @file octree.cpp
 * @author Tiziano Bettio
 * @date 2019-01-29
 */

/* interrogate doesn't need to know about Octree */
#ifndef CPPPARSER
#include "octree.h"
#include "config_module.h"


/**
 * Constructor.
 */

Octree::
Octree(AABB aabb, int max_depth, bool root) {
  _aabb = aabb;
  _max_depth = max_depth;
  _children.resize(8);
  _is_root = root;
}


/**
 * Copy Constructor. Makes a copy of the root node and all child nodes!!
 */

Octree::
Octree(const Octree& oct) {
  _aabb = oct._aabb;
  _max_depth = oct._max_depth;
  _is_root = oct._is_root;
  for (int i = 0; i < 8; i++) {
    if (oct._children[i]) {
      _children[i].reset(new Octree(*oct._children[i]));
    }
  }
}


/**
 * Destructor...
 */

Octree::
~Octree() {
}


/**
 * Add an `id` inside the deepest possible child node for the given `aabb`.
 */

bool Octree::
add_id(const AABB& aabb, unsigned int id) {
  bool r = _aabb.inside(aabb);
  if (!r) {
    geomstore_cat.error() << 
      "Tried to add id with AABB that is not inside this nodes AABB!" 
      << std::endl;
      return false;
  }
  if (_max_depth) {
    Octant oct;
    r = _aabb.get_octant(aabb, oct);
    if (r) {
      /* Go one level deeper */
      if (!_children[oct]) {
        AABB tmp;
        tmp.box = _aabb.box * 0.5f;
        tmp.origin = {
          _aabb.origin[0] + tmp.box[0] * oct_modifiers[oct][0],
          _aabb.origin[1] + tmp.box[1] * oct_modifiers[oct][1],
          _aabb.origin[2] + tmp.box[2] * oct_modifiers[oct][2]
        };

        _children[oct].reset(new Octree(tmp, _max_depth - 1, false));
      }
      return _children[oct]->add_id(aabb, id);
    }
  }
  /* Reached either max depth or the deepest possible child node */
  _ids.push_back(id);
  return true;
}


/**
 * Fills `ids` with all possibly relevant ids that could intersect with
 * `aabb`.
 */

bool Octree::
get_ids(const AABB& aabb, std::vector<unsigned int>& rvec) {
  bool r = _aabb.inside(aabb);
  if (!r && _is_root) {
    geomstore_cat.warning() << 
      "Tried to lookup ids with AABB that is not inside the Octree!" 
      << std::endl;
      return false;
  }
  if (_max_depth) {
    Octant oct;
    r = _aabb.get_octant(aabb, oct);
    if (r) {
      if (_children[oct]) {
        /* We found a single node containing aabb, dig further */
        _children[oct]->get_ids(aabb, rvec);
      }
    }
    else {
      /* We have reached the final node, consider all children from here */
      for (auto& it : _children) {
        if (it) {
          it->get_ids(aabb, rvec);
        }
      }
    }
  }
  /* We want all bigger items on the way to our final node */
  if (_ids.size()) {
    rvec.reserve(rvec.size() + std::distance(_ids.begin(), _ids.end()));
    rvec.insert(rvec.end(), _ids.begin(), _ids.end());
  }
  /* We only return true from the root node and when elements were found */
  if (rvec.size() && _is_root) {
    return true;
  }
  return false;
}

#endif