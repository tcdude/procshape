/**
 * @file vertex.h
 * @author Tiziano Bettio
 * @date 2018-12-29
 */

#ifndef VERTEX_HPP
#define VERTEX_HPP

#include <iostream>
#include <vector>

#include "luse.h"
#include "pnotify.h"

#include "common.h"
#include "triangle.h"
class Triangle;

using namespace std;

/**
 * Simple representation of a vertex in a mesh with 
 * useful functions for edge collapse based on 
 * 
 * Progressive Mesh type Polygon Reduction Algorithm
 * by Stan Melax (c) 1998
 */
class Vertex {
  public:
    LVecBase3f* position;  // location of point in euclidean space
    UnalignedLVecBase4f* color;  // vertex color
    LVecBase3f normal;  // Vertex Normal
    int id;  // place of vertex in original Array
    
    vector<Vertex *> neighbor;  // adjacent vertices
    vector<Triangle *> face;  // adjacent triangles
    float objdist;  // cached cost of collapsing edge
    Vertex *collapse;  // candidate vertex for collapse
    Vertex(LVecBase3f* v, UnalignedLVecBase4f* c, int _id);
    virtual ~Vertex();
    void remove_if_non_neighbor(Vertex *n);
    void add_neighbor(Vertex *n);
};
#endif