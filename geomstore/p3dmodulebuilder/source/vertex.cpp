/**
 * Simple representation of a vertex in a mesh with 
 * useful functions for edge collapse based on 
 * 
 * Progressive Mesh type Polygon Reduction Algorithm
 * by Stan Melax (c) 1998
 */

#include <stdio.h> 
#include <vector>
#include <assert.h>

#include "common.hpp"
#include "vertex.hpp"

std::vector<Vertex *> vertices;


Vertex::
Vertex(LVecBase3f *v, int _id) {
	position = v;
	id = _id;
	vertices.push_back(this);
}

Vertex::
~Vertex(){
	assert(face.size() == 0);
	while(neighbor.size()) {
		remove(neighbor[0]->neighbor, this);
		remove(neighbor, neighbor[0]);
	}
	remove(vertices, this);
}
void Vertex::
remove_if_non_neighbor(Vertex *n) {
	// removes n from neighbor Array if n isn't a neighbor.
	if(!contains(neighbor,n)) return;
	for (unsigned int i = 0; i<face.size(); i++) {
		if(face[i]->has_vertex(n)) return;
	}
	remove(neighbor,n);
}