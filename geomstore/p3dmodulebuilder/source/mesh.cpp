/* interrogate doesn't need to know about OpenMesh */
#ifndef CPPPARSER

#include "mesh.h"

#include <iostream>
#include <vector>
#include <limits>
#include <cmath>

#include <OpenMesh/Tools/Subdivider/Uniform/LoopT.hh>
#include <OpenMesh/Tools/Subdivider/Uniform/LongestEdgeT.hh>

#include "geomstore.h"
#include "config_module.h"


/* public methods */

/**
 * Constructor
 */
Mesh::
Mesh() {

}


/**
 * Destructor
 */
Mesh::
~Mesh() {
}


/**
 * Performs Loop subdivision `c` times.
 */
void Mesh::
subdivision(GeomStore* g, int c) {
  fill_mesh(g);
  OpenMesh::Subdivider::Uniform::LoopT<MyMesh> loop;
  loop.attach(_my_mesh);
  loop(c);
  loop.detach();
  _my_mesh.garbage_collection();
  fill_geomstore(g);
}


/**
 * Performs LongestEdge subdivision until all edge lengths are <= `l`.
 */
void Mesh::
subdivision_length(GeomStore* g, double l) {
  fill_mesh(g);
  OpenMesh::Subdivider::Uniform::LongestEdgeT<MyMesh> loop;
  loop.attach(_my_mesh);
  loop.set_max_edge_length(l);
  loop(1);
  loop.detach();
  _my_mesh.garbage_collection();
  fill_geomstore(g);
}


/**
 * Performs edge collapse on the mesh. TODO: Figure out how to implement.
 */
void Mesh::
decimate(GeomStore* g, float max_err) {
  fill_mesh(g);
}


/**
 * Regenerates the axis aligned bounding box.
 */
void Mesh::
regen_aabb() {
  if (!_filled) {
    geomstore_cat.error() << "Tried to generate aabb while no mesh" 
      << " is loaded." << std::endl;
    return;
  }
  MyMesh::VertexIter v_it, v_end(_my_mesh.vertices_end());
  float xmin, xmax, ymin, ymax, zmin, zmax;
  xmin = ymin = zmin = std::numeric_limits<float>::max();
  xmax = ymax = zmax = std::numeric_limits<float>::min();
  for (v_it = _my_mesh.vertices_begin(); v_it != v_end; ++v_it) {
    xmin = std::min(xmin, _my_mesh.point(*v_it)[0]);
    ymin = std::min(ymin, _my_mesh.point(*v_it)[1]);
    zmin = std::min(zmin, _my_mesh.point(*v_it)[2]);
    xmax = std::max(xmax, _my_mesh.point(*v_it)[0]);
    ymax = std::max(ymax, _my_mesh.point(*v_it)[1]);
    zmax = std::max(zmax, _my_mesh.point(*v_it)[2]);
  }
  aabb.origin = LPoint3f(
    (xmin + xmax) / 2.0f,
    (ymin + ymax) / 2.0f,
    (zmin + zmax) / 2.0f
  );
  aabb.box = LVecBase3f(
    (xmax - xmin) / 2.0f,
    (ymax - ymin) / 2.0f,
    (zmax - zmin) / 2.0f
  );
  aabb.calculated = true;
}


/* private methods */


/**
 * Fill the mesh with the content of GeomStore `g`.
 */
void Mesh::
fill_mesh(GeomStore* g) {
  _my_mesh.clear();
  MyMesh::VertexHandle vhandle[g->_points.size()];
  _my_mesh.request_vertex_colors();
  for (int i = 0; i < g->_points.size(); i++) {
    auto it = g->_points[i];
    auto c = g->_colors[i];
    vhandle[i] = _my_mesh.add_vertex(MyMesh::Point(it[0], it[1], it[2]));
    _my_mesh.set_color(vhandle[i], MyMesh::Color(c[0], c[1], c[2]));
  }
  
  for (auto it : g->_triangles) {
    std::vector<MyMesh::VertexHandle> face_vhandles = {
      vhandle[it[0]],
      vhandle[it[1]],
      vhandle[it[2]],
    };
    _my_mesh.add_face(face_vhandles);
  }
  _filled = true;
}


/**
 * Fill the GeomStore `g` with the content of this mesh.
 */
void Mesh::
fill_geomstore(GeomStore* g) {
  g->clear();
  _my_mesh.request_vertex_colors();
  g->set_num_rows(_my_mesh.n_vertices(), _my_mesh.n_faces());
  MyMesh::VertexIter v_it, v_end(_my_mesh.vertices_end());
  for (v_it = _my_mesh.vertices_begin(); v_it != v_end; ++v_it) {
    g->_points.push_back(LVecBase3f(
      _my_mesh.point(*v_it)[0], 
      _my_mesh.point(*v_it)[1], 
      _my_mesh.point(*v_it)[2]));
    g->_colors.push_back(LVecBase4f(
      _my_mesh.color(*v_it)[0], 
      _my_mesh.color(*v_it)[1], 
      _my_mesh.color(*v_it)[2],
      1.0f));
  }
  MyMesh::FaceIter f_it, f_end(_my_mesh.faces_end());
  MyMesh::FaceVertexCCWIter fvit;
  for (f_it = _my_mesh.faces_begin(); f_it != f_end; ++f_it) {
    LVecBase3i tri = -1;
    int pos = 0;
    for (fvit = _my_mesh.fv_ccwiter(*f_it); fvit.is_valid(); ++fvit) {
      tri[pos] = fvit->idx();
      pos++;
    }

    g->_triangles.push_back(tri);
  }
  _filled = false;
  aabb.calculated = false;
}


#endif
