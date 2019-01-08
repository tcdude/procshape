/**
 * @file triangle.cpp
 * @author Tiziano Bettio
 * @date 2018-12-29
 */

#include "triangle.h"

using namespace std;

/**
 * Simple representation of a triangle in a mesh with 
 * useful functions for edge collapse based on 
 * 
 * Progressive Mesh type Polygon Reduction Algorithm
 * by Stan Melax (c) 1998
 */
Triangle::
Triangle(Vertex *v0, Vertex *v1, Vertex *v2) {
  nassertv(v0 != v1 && v1 != v2 && v2 != v0);
  vertex[0] = v0;
  vertex[1] = v1;
  vertex[2] = v2;
  compute_normal();
	compute_longest_edge();
	for (int i = 0; i < 3; i++) {
		vertex[i]->face.push_back(this);
		vertex[i]->add_neighbor(vertex[(i + 1) % 3]);
		vertex[i]->add_neighbor(vertex[(i - 1) % 3]);
	}
}

Triangle::
~Triangle(){
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
	return (v==vertex[0] || v==vertex[1] || v==vertex[2]);
}

void Triangle::
compute_normal()
{
	LVecBase3f v0 = *vertex[0]->position;
	LVecBase3f v1 = *vertex[1]->position;
	LVecBase3f v2 = *vertex[2]->position;
	normal = (v1 - v0).cross((v2 - v1));
	if(length(normal) == 0)return;
	normal = normal.normalized();
}

void Triangle::
compute_longest_edge()
{
	float max_dist = 0.0;
	_longest_edge_index = -1;
	for (int i = 0; i < 3; i++) {
		LVecBase3f v = *vertex[(i + 1) % 3]->position - *vertex[i]->position;
		float dist = v.length();
		if (dist > max_dist) {
			max_dist = dist;
			_longest_edge_index = i;
		}
	}
	_longest_edge_length = max_dist;
	nassertv(_longest_edge_index > -1 && _longest_edge_index < 3);
}

void Triangle::
replace_vertex(Vertex *v_old, Vertex *v_new) 
{
	nassertv(v_old && v_new);
	nassertv(v_old == vertex[0] || v_old == vertex[1] || v_old == vertex[2]);
	nassertv(v_new != vertex[0] && v_new != vertex[1] && v_new != vertex[2]);
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
	nassertv(!contains_(v_new->face, this));
	v_new->face.push_back(this);
	for (int i = 0; i < 3; i++) {
		v_old->remove_if_non_neighbor(vertex[i]);
		vertex[i]->remove_if_non_neighbor(v_old);
	}
	for (int i = 0; i < 3; i++) {
		nassertv(contains_(vertex[i]->face, this) == 1);
		for(int j = 0; j < 3 ;j++) if( i!= j) {
			add_unique(vertex[i]->neighbor, vertex[j]);
		}
	}
	compute_normal();
	compute_longest_edge();
} 