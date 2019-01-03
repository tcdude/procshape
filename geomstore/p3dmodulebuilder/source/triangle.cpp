/**
 * Simple representation of a triangle in a mesh with 
 * useful functions for edge collapse based on 
 * 
 * Progressive Mesh type Polygon Reduction Algorithm
 * by Stan Melax (c) 1998
 */

#include <stdio.h> 
#include <vector>
#include <assert.h>

#include "triangle.hpp"
#include "common.hpp"

std::vector<Triangle *> triangles;

Triangle::
Triangle(Vertex *v0, Vertex *v1, Vertex *v2) {
  assert(v0 != v1 && v1 != v2 && v2 != v0);
  vertex[0] = v0;
  vertex[1] = v1;
  vertex[2] = v2;
  compute_normal();
  triangles.push_back(this);
  for(int i = 0; i < 3; i++) {
    vertex[i];
  }
}