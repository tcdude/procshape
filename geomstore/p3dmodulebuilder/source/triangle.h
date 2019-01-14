/**
 * @file triangle.h
 * @author Tiziano Bettio
 * @date 2018-12-29
 */

#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP


#include <algorithm>
#include <iostream>
#include <vector>
#include <cmath>

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
    ~Triangle();
    bool update();
    bool has_vertex(Vertex *v);
    bool replace_vertex(Vertex *v_old, Vertex *v_new);
    bool flip();
    LVecBase3f get_split_point();
    LVecBase4f get_split_color();
    Vertex *vertex[3];  // the 3 points that make this tri
    LVecBase3f normal;  // unit vector orthogonal to this face
    int _longest_edge_index; // the start index of the longest edge
    float _longest_edge_length; // the length of the longest edge
    float _area; // the area of the triangle
    bool operator < (const Triangle& t);
  private:
    bool compute_normal();
    bool compute_split();
};

#endif