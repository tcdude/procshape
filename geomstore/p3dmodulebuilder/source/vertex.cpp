/**
 * @file vertex.cpp
 * @author Tiziano Bettio
 * @date 2018-12-29
 */

#include "vertex.h"

/**
 * Simple representation of a vertex in a mesh with 
 * useful functions for edge collapse based on 
 * 
 * Progressive Mesh type Polygon Reduction Algorithm
 * by Stan Melax (c) 1998
 */
Vertex::
Vertex(LVecBase3f* v, UnalignedLVecBase4f* c, int _id) {
	position = v;
	color = c;
	id = _id;
}

Vertex::
~Vertex(){
	nassertv(face.size() == 0);
	while(neighbor.size()) {
		remove(neighbor[0]->neighbor, this);
		remove(neighbor, neighbor[0]);
	}
}

void Vertex::
remove_if_non_neighbor(Vertex *n) {
	// removes n from neighbor Array if n isn't a neighbor.
	if(!contains(neighbor,n)) return;
	for (unsigned int i = 0; i < face.size(); i++) {
		if(face[i]->has_vertex(n)) return;
	}
	remove(neighbor, n);
}