#include <iostream>
#include <vector>

#include "geomstore.h"



GeomStore::
GeomStore() {

}

GeomStore::
~GeomStore() {

}

int GeomStore::
add_vertex(LVecBase3f v, LVecBase4f c) {
  return 0;
}

int GeomStore:: 
add_triangle(int v0, int v1, int v2) {
  return 0;
}

void GeomStore::
subdivide_triangles(int s) {

}

void GeomStore::
subdivide_triangles_distance(float d) {

}

void GeomStore::
extend(GeomStore *other) {

}
