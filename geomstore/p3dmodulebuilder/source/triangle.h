/**
 * @file triangle.h
 * @author Tiziano Bettio
 * @date 2018-12-29
 */

#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP


#include <iostream>
#include <vector>

#include "pnotify.h"
#include "luse.h"

#include "common.h"
#include "vertex.h"
class Vertex;

/**
 * Simple representation of a triangle in a mesh with 
 * useful functions for edge collapse based on 
 * 
 * Progressive Mesh type Polygon Reduction Algorithm
 * by Stan Melax (c) 1998
 */
class Triangle {
  public:
    Triangle(Vertex *v0, Vertex *v1, Vertex *v2);
    virtual ~Triangle();
    Vertex *vertex[3];  // the 3 points that make this tri
    LVecBase3f normal;  // unit vector orthogonal to this face
    void compute_normal();
    void replace_vertex(Vertex *v_old, Vertex *v_new);
    int has_vertex(Vertex *v);
};

#endif