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

#include "triangle.h"
#include "common.h"

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

Triangle::
~Triangle(){
	remove(triangles, this);
	for(int i = 0; i < 3; i++) {
		if(vertex[i]) remove(vertex[i]->face, this);
	}
	for (int i = 0; i < 3; i++) {
		int i2 = (i + 1) % 3;
		if(!vertex[i] || !vertex[i2]) continue;
		vertex[i]->remove_if_non_neighbor(vertex[i2]);
		vertex[i2]->remove_if_non_neighbor(vertex[i]);
	}
}

int Triangle::
has_vertex(Vertex *v) {
	return (v==vertex[0] ||v==vertex[1] || v==vertex[2]);
}

void Triangle::
compute_normal()
{
	LVecBase3f v0 = vertex[0]->position;
	LVecBase3f v1 = vertex[1]->position;
	LVecBase3f v2 = vertex[2]->position;
	normal = (v1 - v0).cross((v2 - v1));
	if(length(normal) == 0)return;
	normal = normal.normalized();
}

void Triangle::
replace_vertex(Vertex *v_old, Vertex *v_new) 
{
	assert(v_old && v_new);
	assert(v_old == vertex[0] || v_old == vertex[1] || v_old == vertex[2]);
	assert(v_new != vertex[0] && v_new != vertex[1] && v_new != vertex[2]);
	if(v_old == vertex[0]){
		vertex[0] = v_new;
	}
	else if(v_old == vertex[1]){
		vertex[1] = v_new;
	}
	else {
		assert(v_old == vertex[2]);
		vertex[2] = v_new;
	}
	remove(v_old->face, this);
	assert(!contains(v_new->face, this));
	v_new->face.push_back(this);
	for (int i = 0; i < 3; i++) {
		v_old->remove_if_non_neighbor(vertex[i]);
		vertex[i]->remove_if_non_neighbor(v_old);
	}
	for (int i = 0; i < 3; i++) {
		assert(contains(vertex[i]->face, this) == 1);
		for(int j=0; j < 3 ;j++) if( i!= j) {
			add_unique(vertex[i]->neighbor,vertex[j]);
		}
	}
	compute_normal();
}