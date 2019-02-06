 /**
 * @file mesh.cpp
 * @author Tiziano Bettio
 * @date 2019-01-20
 */

/* interrogate doesn't need to know about OpenMesh */
#ifndef CPPPARSER

#include "mesh.h"

#include <iostream>
#include <limits>
#include <cmath>

#include <OpenMesh/Tools/Subdivider/Uniform/LoopT.hh>
#include <OpenMesh/Tools/Subdivider/Uniform/LongestEdgeT.hh>

#include "geomstore.h"
#include "config_module.h"
#include "csg.h"


/**
 * Computes the coordinates of the vertices of a 3D polygon
 * in a local 2D orthonormal basis of the triangle's plane.
 */

void project_polygon(std::vector<Vec3f>& poly3, std::vector<Vec2f>& poly2) {
  nassertv(poly3.size() > 2);
  poly2.clear();
  poly2.reserve(poly3.size());
  Vec3f x_a, y_a, z_a;
  x_a = poly3[1] - poly3[0];
  x_a.normalize();
  z_a = OpenMesh::cross(x_a, (poly3[2] - poly3[0]));
  z_a.normalize();
  y_a = OpenMesh::cross(z_a, x_a);
  poly2.push_back(Vec2f(0.0f)); 
  const Vec3f& origin = poly3[0];
  poly2.push_back(Vec2f((poly3[1] - origin).length(), 0.0f));

  for (std::vector<Vec3f>::size_type i = 2; i < poly3.size(); ++i) {
    Vec3f p = poly3[i] - origin;
    float x = OpenMesh::dot(p, x_a);
    float y = OpenMesh::dot(p, y_a);
    poly2.push_back(Vec2f(x, y));
  }
}



/**
 * Constructor. Specify wheter the mesh is an `outside` mesh. 
 * Default is true.
 */
Mesh::
Mesh(bool outside) {
  _my_mesh.add_property(_processed);
  _my_mesh.add_property(_outside);
  _my_mesh.property(_outside) = outside;
  _my_mesh.add_property(_tri_aabb);
  _my_mesh.add_property(_tri_intersection);
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
 * Extends this Mesh with `other`.
 */

void Mesh::
extend(Mesh& other, bool inv_winding) {
  _my_mesh.reserve(
    _my_mesh.n_vertices() + other._my_mesh.n_vertices(), 
    _my_mesh.n_edges() + other._my_mesh.n_edges(), 
    _my_mesh.n_faces() + other._my_mesh.n_faces());
  MyMesh::VertexHandle vhandle[other._my_mesh.n_vertices()];
  _my_mesh.request_vertex_colors();
  other._my_mesh.request_vertex_colors();

  MyMesh::VertexIter v_it, v_end = other._my_mesh.vertices_end();
  for (v_it = other._my_mesh.vertices_begin(); v_it != v_end; ++v_it) {
    MyMesh::Point p = other._my_mesh.point(*v_it);
    MyMesh::Color c = other._my_mesh.color(*v_it);
    MyMesh::VertexIter tv_it = _my_mesh.vertices_begin();
    MyMesh::VertexIter tv_end = _my_mesh.vertices_end();
    bool isnew = true;
    for (tv_it; tv_it != tv_end; ++tv_it) {
      MyMesh::Point tp = _my_mesh.point(*tv_it);
      MyMesh::Color tc = _my_mesh.color(*tv_it);
      if (p == tp && c == tc) {
        isnew = false;
        vhandle[v_it->idx()] = *tv_it;
        break;
      }
    }

    if (isnew) {
      vhandle[v_it->idx()] = _my_mesh.add_vertex(p);
      _my_mesh.set_color(vhandle[v_it->idx()], c);
    }
  }

  MyMesh::FaceIter f_it, f_end = other._my_mesh.faces_end();
  for (f_it = other._my_mesh.faces_end(); f_it != f_end; ++f_it) {
    std::vector<MyMesh::VertexHandle> vhvec;
    if (inv_winding) {
      MyMesh::FaceVertexCWIter fv_it = other._my_mesh.fv_cwiter(*f_it);
      for (fv_it; fv_it.is_valid; ++fv_it) {
        vhvec.push_back(vhandle[fv_it->idx()]);
      }
    }
    else {
      MyMesh::FaceVertexCCWIter fv_it = other._my_mesh.fv_ccwiter(*f_it);
      for (fv_it; fv_it.is_valid; ++fv_it) {
        vhvec.push_back(vhandle[fv_it->idx()]);
      }
    }
    _my_mesh.add_face(vhvec);
  }

  _filled = true;
  _dirty = true;

}


/**
 * CSG:
 * Only information about the mesh, that also could be otherwise of use
 * will be handled inside Mesh (e.g. AABB computation), all the rest
 * happens in CSG. Mesh must expose only three methods: 
 *    void union(Mesh& other)
 *    void difference(Mesh& other)
 *    void intersect(Mesh& other)
 * which in turn call their corresponding methods of CSG after eventual
 * pre processing.
 */

/**
 * Performs CSG Union on this with `other`
 */

void Mesh::
mesh_union(Mesh& other) {
  fill_octree();
  other.fill_octree();
  Mesh out;
  csg_union(*this, other, out);
  replace(out);
}


/**
 * Performs CSG Difference on this with `other`
 */

void Mesh::
mesh_difference(Mesh& other) {
  fill_octree();
  other.fill_octree();
  Mesh out;
  csg_difference(*this, other, out);
  replace(out);
}


/**
 * Performs CSG Union on this with `other`
 */

void Mesh::
mesh_intersect(Mesh& other) {
  fill_octree();
  other.fill_octree();
  Mesh out;
  csg_intersect(*this, other, out);
  replace(out);
}


/**
 * Fills the octree for this mesh.
 */

void Mesh::
fill_octree() {
  regen_aabb();
  _octree.reset(new Octree(aabb));
  MyMesh::FaceIter f_it, f_end = _my_mesh.faces_end();
  for (f_it = _my_mesh.faces_begin(); f_it != f_end; ++f_it) {
    _my_mesh.property(_tri_aabb, *f_it) = face_aabb(*f_it);
    _octree->add_id(_my_mesh.property(_tri_aabb, *f_it), f_it->idx());
  }
}


/**
 * Returns the AABB for the specified FaceHandle `fh`.
 */

AABB Mesh::
face_aabb(const MyMesh::FaceHandle& fh) {
  MyMesh::FaceVertexIter fv_it = _my_mesh.fv_iter(fh);
  LVecBase3f p_max = {std::numeric_limits<float>::min()};
  LVecBase3f p_min = {std::numeric_limits<float>::max()};
  for (fv_it; fv_it.is_valid(); ++fv_it) {
    MyMesh::Point p = _my_mesh.point(*fv_it);
    p_max[0] = std::max(p_max[0], p[0]);
    p_max[1] = std::max(p_max[1], p[1]);
    p_max[2] = std::max(p_max[2], p[2]);
    p_min[0] = std::min(p_min[0], p[0]);
    p_min[1] = std::min(p_min[1], p[1]);
    p_min[2] = std::min(p_min[2], p[2]);
  }
  AABB f_aabb;
  f_aabb.box = (p_max - p_min) * 0.5f;
  f_aabb.origin = (LPoint3f) p_min + f_aabb.box;
  return f_aabb;
}


/**
 * Replaces the current mesh with `other`
 */

void Mesh::
replace(Mesh& other) {
  _my_mesh = other._my_mesh;
  _dirty = true;
  _octree = nullptr;
}


/**
 * Returns true if at least one point is inside the others AABB otherwise false. 
 */
bool Mesh::
has_points_inside(Mesh& other) {
  MyMesh::VertexIter v_it, v_end(_my_mesh.vertices_end());
  for (v_it = _my_mesh.vertices_begin(); v_it != v_end; ++v_it) {
    MyMesh::Point p = _my_mesh.point(*v_it);
    if (other.aabb.inside(p)) {
      return true;
    }
  }
  return false;
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
  if (!_dirty) {
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
  aabb.box *= 1.001;  // Prevent precision error
  aabb.calculated = true;
  _dirty = false;
}


/**
 * Returns the octree of this mesh, filling it first, if the mesh
 * is marked as _dirty or _octree hasn't been populated yet.
 */
Octree Mesh::
get_octree() {
  if (!_octree) {
    fill_octree();
  }
  return *_octree;
}


/* private methods */


/**
 * Fill the mesh with the content of GeomStore `g`.
 */
void Mesh::
fill_mesh(GeomStore* g) {
  _my_mesh.clear();
  _my_mesh.reserve(
    g->_points.size(), 
    g->_triangles.size() * 3, 
    g->_triangles.size());
  MyMesh::VertexHandle vhandle[g->_points.size()];
  _my_mesh.request_vertex_colors();
  for (int i = 0; i < g->_points.size(); ++i) {
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
  _dirty = true;
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
