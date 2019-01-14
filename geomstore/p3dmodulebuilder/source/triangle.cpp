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
	if(v0 == v1 || v1 == v2 || v2 == v0) {
		geomstore_cat->error() 
			<< "Must be 3 individual vertices! Vertices([" << v0->id << "]@'"
			<< &v0 << "'/[" << v1->id << "]@'" << &v1 << "'/[" << v2->id << "]@'" 
			<< &v2 << "') were passed in!!" << endl;
	}
  nassertv(v0 != v1 && v1 != v2 && v2 != v0);
  vertex[0] = v0;
  vertex[1] = v1;
  vertex[2] = v2;
	update();
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

/**
 * Update face normal and split information.
 */
bool Triangle::
update() {
	nassertr(compute_normal(), false);
	nassertr(compute_split(), false);
	return true;	
}

/**
 * Returns true if v is part of this face.
 */
bool Triangle::
has_vertex(Vertex *v) {
	return (v==vertex[0] || v==vertex[1] || v==vertex[2]);
}

/**
 * Replaces vertex v_old in this face with v_new.
 */
bool Triangle::
replace_vertex(Vertex *v_old, Vertex *v_new) 
{
	nassertr(v_old && v_new, false);
	nassertr(v_old == vertex[0] || v_old == vertex[1] || v_old == vertex[2], false);
	nassertr(v_new != vertex[0] && v_new != vertex[1] && v_new != vertex[2], false);
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
	nassertr(!contains_(v_new->face, this), false);
	v_new->face.push_back(this);
	for (int i = 0; i < 3; i++) {
		v_old->remove_if_non_neighbor(vertex[i]);
		vertex[i]->remove_if_non_neighbor(v_old);
	}
	for (int i = 0; i < 3; i++) {
		nassertr(contains_(vertex[i]->face, this) == 1, false);
		for(int j = 0; j < 3 ;j++) if( i!= j) {
			add_unique(vertex[i]->neighbor, vertex[j]);
		}
	}
	update();
	return true;
} 


/**
 * Flips the winding of this triangle.
 */
bool Triangle::
flip() {
	swap(vertex[0], vertex[2]);
	nassertr(update(), false);
	return true;
}

/**
 * Returns the point where the triangle should be split.
 */
LVecBase3f Triangle::
get_split_point() {
	LVecBase3f p = *vertex[(_longest_edge_index + 1) % 3]->position - 
								 *vertex[_longest_edge_index]->position;
	p = p * 0.5f + *vertex[_longest_edge_index]->position;
	return p;
}


/**
 * Returns the point where the triangle should be split.
 */
LVecBase4f Triangle::
get_split_color() {
	LVecBase4f c = (LVecBase4f) *vertex[(_longest_edge_index + 1) % 3]->color + 
								 (LVecBase4f) *vertex[_longest_edge_index]->color;
	c *= 0.5f;
	return c;
}


/**
 * Compare by area.
 */
bool Triangle::
operator < (const Triangle& t) {
	if (_area < t._area) {
		return true;
	}
	return false;
}


/**
 * Compute orthogonal unit length vector of this face.
 */
bool Triangle::
compute_normal()
{
	LVecBase3f v0 = *vertex[0]->position;
	LVecBase3f v1 = *vertex[1]->position;
	LVecBase3f v2 = *vertex[2]->position;
	normal = (v1 - v0).cross((v2 - v1));
	if(length(normal) == 0) {
		return false;
	}
	normal = normal.normalized();
	return true;
}


/**
 * Compute area^2 and longest edge for triangle split
 */
bool Triangle::
compute_split()
{
	float d[3];
	for (int i = 0; i < 3; i++) {
		LVecBase3f v = *vertex[(i + 1) % 3]->position - *vertex[i]->position;
		d[i] = v.dot(v);
	}
	/* This is area^2. Only needed for comparison and saves 3 sqrt calls. */
	_area = (2 * d[0] * d[1] + 2 * d[1] * d[2] + 2 * d[2] * d[0]
					 - d[0] * d[0] - d[1] * d[1] - d[2] * d[2]) / 16.0f;
	
	_longest_edge_length = sqrtf(*max_element(d, d + 3));
	_longest_edge_index = distance(d, max_element(d, d + 3));
	return true;
}

