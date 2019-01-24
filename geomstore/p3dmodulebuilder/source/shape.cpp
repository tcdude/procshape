/**
 * @file shape.cpp
 * @author Tiziano Bettio
 * @date 2018-01-10
 */

#include <iostream>
#include <cmath>

#include "nodePath.h"
#include "luse.h"

#include "config_module.h"
#include "geomstore.h"
#include "common.h"

#include "shape.h"


using namespace std;

/**
 * Evaluates a bounding box and sets the corresponding indices.
 */
void set_dimensions(LVecBase3f bb, int *dim_max, int *dim_min, int *dim_other) {
  if (bb[0] >= bb[1] && bb[0] >= bb[2]) {
    *dim_max = 0;
    if (bb[1] >= bb[2]) {
      *dim_min = 2;
    }
    else {
      *dim_min = 1;
    }
  }
  else if (bb[1] >= bb[0] && bb[1] >= bb[2]) {
    *dim_max = 1;
    if (bb[0] >= bb[2]) {
      *dim_min = 2;
    }
    else {
      *dim_min = 0;
    }
  }
  else {
    *dim_max = 2;
    if (bb[0] >= bb[1]) {
      *dim_min = 1;
    }
    else {
      *dim_min = 0;
    }
  }
  *dim_other = 3 - *dim_max - *dim_min;
}


/**
 * Returns closest multiple of 4, for how many times `l` fits onto the perimeter
 * of the ellipse with sides `a` and `b`.
 */
int pts_on_ellipse(float a, float b, float l) {
  float p = ellipse_perimeter(a, b);
  int num_points = (int) ceil(p / l);
  int r = num_points % 4;
  if (r == 0) {
    return num_points;
  }
  if (r < 3) {
    num_points += r;  
  }
  else {
    num_points -= r;
    num_points = max(num_points, 4);  // probably not needed!!
  }
  return num_points;
}


Shape::
Shape() {

}

Shape::
~Shape() {

}

/**
 * Returns a GeomStore of a plane.
 * 
 * Args:
 *  LVecBase3f origin: center point of the plane in local space.
 *  LVecBase2f bounds: 2D bounds of the plane.
 *  LVecBase3f normal: normal vector, orthogonal to the plane.
 */
GeomStore Shape::
get_plane(LVecBase3f origin, LVecBase2f bounds, LVecBase3f normal, bool two_sided) {
  GeomStore g;
  g.set_num_rows(4);
  int dim_max;
  LVecBase3f u = {0.0f, 0.0f, 0.0f};
  if (normal[0] >= normal[1] && normal[0] >= normal[2]) {
    /* x */
    dim_max = 0;
    u[1] = 1.0f;
  }
  else if (normal[1] >= normal[0] && normal[1] >= normal[2]) {
    /* y */
    dim_max = 1;
    u[0] = -1.0f;
  }
  else {
    /* z */
    dim_max = 2;
    u[0] = 1.0f;
  }
  if (normal[dim_max] < 0) {
    u *= -1.0f;
  }
  LVecBase3f v_tangent = normal.cross(u);
  LVecBase3f u_tangent = normal.cross(-v_tangent);
  u_tangent *= bounds[0];
  v_tangent *= bounds[1];
  LVecBase3f p1 = origin - u_tangent - v_tangent;
  LVecBase3f p2 = origin + u_tangent - v_tangent;
  LVecBase3f p3 = origin + u_tangent + v_tangent;
  LVecBase3f p4 = origin - u_tangent + v_tangent;
  int p1_id = g.add_vertex(p1);
  int p2_id = g.add_vertex(p2);
  int p3_id = g.add_vertex(p3);
  int p4_id = g.add_vertex(p4);
  g.add_triangle(p1_id, p2_id, p3_id);
  g.add_triangle(p3_id, p4_id, p1_id);
  if (two_sided) {
    g.add_triangle(p2_id, p1_id, p3_id);
    g.add_triangle(p3_id, p1_id, p4_id);
  }
  return g;
}

/**
 * Returns a GeomStore of a cuboid.
 * 
 * Args:
 *  LVecBase3f bounding_box: 3D bounding box of the cuboid.
 *  LVecBase3f origin_offset: center point offset of the cuboid in local space.
 */
GeomStore Shape::
get_cuboid(LVecBase3f bounding_box, LVecBase3f origin_offset) {
  GeomStore g;
  g.set_num_rows(8, 12);
  LVecBase3f up_left_front = {-bounding_box[0], -bounding_box[1], bounding_box[2]};
  LVecBase3f down_left_front = {-bounding_box[0], -bounding_box[1], -bounding_box[2]};
  LVecBase3f down_right_front = {bounding_box[0], -bounding_box[1], -bounding_box[2]};
  LVecBase3f up_right_front = {bounding_box[0], -bounding_box[1], bounding_box[2]};
  LVecBase3f up_left_back = {-bounding_box[0], bounding_box[1], bounding_box[2]};
  LVecBase3f down_left_back = {-bounding_box[0], bounding_box[1], -bounding_box[2]};
  LVecBase3f down_right_back = {bounding_box[0], bounding_box[1], -bounding_box[2]};
  LVecBase3f up_right_back = {bounding_box[0], bounding_box[1], bounding_box[2]};
  int p_ulf = g.add_vertex(up_left_front);
  int p_dlf = g.add_vertex(down_left_front);
  int p_drf = g.add_vertex(down_right_front);
  int p_urf = g.add_vertex(up_right_front);
  int p_ulb = g.add_vertex(up_left_back);
  int p_dlb = g.add_vertex(down_left_back);
  int p_drb = g.add_vertex(down_right_back);
  int p_urb = g.add_vertex(up_right_back);

  /* Front */
  g.add_quad(p_ulf, p_dlf, p_drf, p_urf);
  /* Back */
  g.add_quad(p_ulb, p_urb, p_drb, p_dlb);
  /* Up */
  g.add_quad(p_ulb, p_ulf, p_urf, p_urb);
  /* Down */
  g.add_quad(p_dlb, p_drb, p_drf, p_dlf);
  /* Left */
  g.add_quad(p_ulb, p_dlb, p_dlf, p_ulf);
  /* Right */
  g.add_quad(p_urb, p_urf, p_drf, p_drb);
  g.subdivide_triangles_distance(1.5 * min(bounding_box[0], min(bounding_box[1], bounding_box[2])));
  return g;
}

/**
 * Returns a GeomStore of a spheroid. Creates an uneven number (but at least 
 * one) of diamond shaped polygons, orthogonal to the longest axis of the 
 * specified bounding box with single points at both extremes on said axis.
 * Therefore achieving less distortion when subsequently subdividing triangles.
 * 
 * Args:
 *  LVecBase3f bounding_box: 3D bounding box of the spheroid (its max extent).
 *  LVecBase3f origin_offset: center point offset of the spheroid in local space.
 */
GeomStore Shape::
get_spheroid(LVecBase3f bounding_box, LVecBase3f origin_offset) {
  GeomStore g;
  int dim_max;
  int dim_min;
  int dim_other;
  set_dimensions(bounding_box, &dim_max, &dim_min, &dim_other);
  geomstore_cat->warning() << "Compute number of polygons to create" << endl;
  float min_len = ellipse_perimeter(
    bounding_box[dim_min], 
    bounding_box[dim_other]
  );
  float max_len = ellipse_perimeter(
    bounding_box[dim_max], 
    bounding_box[dim_other]
  );
  int num_poly = (int) round(max_len / min_len); 
  geomstore_cat->warning() << "Number of polygons until singularity: " 
    << num_poly << endl;
  g.set_num_rows(num_poly * 2 + 1);

  vector<int> v_id(num_poly * 2);
  geomstore_cat->warning() << "Create Vertices" << endl;
  for (int i = 0; i < num_poly; i++) {
    float a, b, ppos;
    int dim_x, dim_y;
    dim_x = (dim_max + 1) % 3;
    dim_y = (dim_max + 2) % 3;
    if (i > 0) {
      //ppos = bounding_box[dim_max] * pow((float) i / (float) num_poly, 1.618f);
      ppos = bounding_box[dim_max] / (float) num_poly * (float) i;
      float f = (1.0f - pow(ppos, 2.0f) / pow(bounding_box[dim_max], 2.0f));
      a = sqrt(pow(bounding_box[dim_x], 2.0f) * f);
      b = sqrt(pow(bounding_box[dim_y], 2.0f) * f);
    }
    else {
      ppos = 0.0f;
      a = bounding_box[dim_x];
      b = bounding_box[dim_y];
    }

    LVecBase3f p1 = 0.0f;
    LVecBase3f p2 = 0.0f;
    p1[dim_max] = ppos;
    p1[dim_y] = b;
    p2[dim_max] = ppos;
    p2[dim_x] = a;

    v_id[i * 2] = g.add_vertex(p1);
    v_id[i * 2 + 1] = g.add_vertex(p2);
  }
  geomstore_cat->warning() << "v_id holds: " << vec_str(v_id) << endl;

  if (num_poly > 1) {
    geomstore_cat->warning() << "Connect with Quads" << endl;
    for (int i = 0; i < num_poly - 1; i++) {
      nassertr(
        g.add_quad(v_id[i + 3], v_id[i + 1], v_id[i], v_id[i + 2]) != -1, 
        g
      );
    }
  }
  
  geomstore_cat->warning() << "Connect with Singularity" << endl;
  LVecBase3f p = 0.0f;
  p[dim_max] = bounding_box[dim_max];
  int end_id = g.add_vertex(p);
  nassertr(
    g.add_triangle(v_id[num_poly * 2 - 1], v_id[num_poly * 2 - 2], end_id) != -1, 
    g
  );

  geomstore_cat->warning() << "Mirroring over axis " << min(dim_min, dim_other)
    << " containing verts/tris:" << endl;
  g.print_vertices();  
  g.print_triangles();  
  g.mirror(min(dim_min, dim_other));

  geomstore_cat->warning() << "Mirroring over axis " << max(dim_min, dim_other)
    << " containing verts/tris:" << endl;
  g.print_vertices();  
  g.print_triangles();  
  g.mirror(max(dim_min, dim_other));

  geomstore_cat->warning() << "Mirroring over axis " << dim_max
    << " containing verts/tris:" << endl;
  g.print_vertices();  
  g.print_triangles();  
  g.mirror(dim_max);
  return g;
}

/**
 * EXPERIMENTAL! NOT WORKING (YET)... TODO: Find a better way to keep vertex
 * count low!
 * 
 * Returns a GeomStore of a spheroid. Creates an uneven number of ellipse-like 
 * polygons, orthogonal to the longest axis of the specified bounding box with 
 * single points at both extremes on said axis. Each polygon gets n sides where 
 * n is an integer that is the result of dividing the approximated perimeters 
 * of the largest ellipse by the quarter length of the smallest ellipse in the 
 * bounding box, rounded to the nearest multiple of 4 to insure one point for 
 * each axis extreme. Finally creates triangles from the resulting vertices.
 * 
 * Args:
 *  LVecBase3f bounding_box: 3D bounding box of the spheroid (its max extent).
 *  LVecBase3f origin_offset: center point offset of the spheroid in local space.
 */
GeomStore Shape::
get_spheroid_exp(LVecBase3f bounding_box, LVecBase3f origin_offset) {
  GeomStore g;
  int dim_max;
  int dim_min;
  int dim_other;
  set_dimensions(bounding_box, &dim_max, &dim_min, &dim_other);
  geomstore_cat->warning() << "Compute number of ellipses to create" << endl;
  float min_len = ellipse_perimeter(bounding_box[dim_min], bounding_box[dim_other]);
  float max_len = ellipse_perimeter(bounding_box[dim_max], bounding_box[dim_other]);
  float other_len = ellipse_perimeter(bounding_box[dim_max], bounding_box[dim_min]);
  vector<int> pts = {
    4, 
    (int) round((max_len / min_len) * 4.0f), 
    (int) round((other_len / min_len) * 4.0f)
  };
  int lcm_pts = lcm_vect(pts);
  float base_len = max_len / (float) lcm_pts;

  int num_poly = lcm_pts / 4; 
  geomstore_cat->warning() << "Number of polygons until singularity: " 
    << num_poly << endl;
  const int poly_pts_max = (int) ceil(min_len / base_len / 4.0f); 
  vector<vector<int>> v_id(num_poly);  // vertex ids
  for (int i = 0; i < num_poly; i++) {
    v_id[i].reserve(poly_pts_max);
  }
  float p_len[num_poly];  // all polygon perimeters

  geomstore_cat->warning() << "Make Polygon Quarts" << endl;
  for (int i = 0; i < num_poly; i++) {
    float ppos = (float) i * bounding_box[dim_max] / (float) num_poly;
    float a, b;
    int dim_x, dim_y;
    dim_x = (dim_min < dim_other) ? dim_min : dim_other;
    dim_y = (dim_min < dim_other) ? dim_other : dim_min;
    if (ppos == 0.0f) {
      a = bounding_box[dim_x];
      b = bounding_box[dim_y];
    }
    else {
      a = bounding_box[dim_x] * sqrt(1.0f - pow(ppos / bounding_box[dim_max], 2.0f));
      b = bounding_box[dim_y] * sqrt(1.0f - pow(ppos / bounding_box[dim_max], 2.0f));
    }
    p_len[i] = ellipse_perimeter(a, b);
    int loc_pts = pts_on_ellipse(a, b, base_len) / 4 + 1;
    float rad_step = HALFPI / (float) (loc_pts - 1);
    /* Add all positive parts points of the ellipse */
    for (int j = 0; j < loc_pts; j++) {
      LVecBase3f p;
      p[dim_max] = ppos;
      p[dim_x] = a * (round(sin(rad_step * j) * 1e6f) / 1e6f);
      p[dim_y] = b * (round(cos(rad_step * j) * 1e6f) / 1e6f);
      v_id[i].push_back(g.add_vertex(p));
    }
  }

  geomstore_cat->warning() << "Connect Quarter Polygons with Triangles/Quads" << endl;
  for (int i = 0; i < num_poly - 1; i++) {
    if (v_id[i].size() == v_id[i + 1].size()) {
      geomstore_cat->warning() << "Generate quads" << endl;
      for (int j = 0; j < v_id[i].size(); j++) {
        int t_id = g.add_quad(
          v_id[i][j], 
          v_id[i][j + 1], 
          v_id[i + 1][j + 1], 
          v_id[i + 1][j]
        );
        if (t_id == -1) {
          geomstore_cat->error() << "Error adding quad with vertex ids (" 
          << v_id[i][j] << "/" << v_id[i][j + 1] << "/" << v_id[i + 1][j + 1] 
          << "/" << v_id[i + 1][j] << ")" << endl;
        }
        nassertr(t_id != -1, g);
      }
    }
    else if (v_id[i].size() > v_id[i + 1].size()) {
      geomstore_cat->warning() << "Generate Linearly spaced triangles" << endl;
      int size0 = v_id[i].size();
      int size1 = v_id[i + 1].size();
      int pos0 = 0;
      int pos1 = 0;
      float seg0 = p_len[i] / (float) size0;
      float seg1 = p_len[i + 1] / (float) size1;
      while (pos0 < size0 - 1 && pos1 < size1 - 1) {
        int t_id;
        if (pos0 * seg0 <= pos1 * seg1) {
          /* Remove first point from row 0 */
          t_id = g.add_triangle(
            v_id[i][pos0], 
            v_id[i][pos0 + 1], 
            v_id[i + 1][pos1]);
          pos0++;
        }
        else {
          /* Remove first point from row 1 */
          t_id = g.add_triangle(
            v_id[i + 1][pos1 + 1], 
            v_id[i + 1][pos1], 
            v_id[i][pos0]);
          pos1++;
        }
        nassertr(t_id != -1, g);
      }
    }
  }

  geomstore_cat->warning() << "Connect last polygon with singularity" << endl;
  LVecBase3f end_point = 0.0f;
  end_point[dim_max] = bounding_box[dim_max];
  int end_point_id = g.add_vertex(end_point);
  for (int i = 0; i < v_id[num_poly - 1].size() - 1; i++) {
    int t_id = g.add_triangle(
      v_id[num_poly - 1][i],
      v_id[num_poly - 1][i + 1],
      end_point_id
    );
    nassertr(t_id != -1, g);
  }

  /* Mirror GeomStore on all axes */
  geomstore_cat->warning() << "Mirroring Spheroid" << endl;
  g.mirror(0);
  g.mirror(1);
  g.mirror(2);
  return g;
}

/**
 * Returns a GeomStore of a cylinder or a cone, if one of the two radii is 
 * set to 0.0f. Tries to approximate homogenous triangles.
 * 
 * Args:
 *  int circle_points: number of points that make the base circle of the cylinder. 
 *  LVecBase3f start_point: start point of the cylinder.
 *  float start_radius: radius of the cylinder at the start point.
 *  LVecBase3f end_point: end point of the cylinder.
 *  float end_radius: radius of the cylinder at the end point.
 */
GeomStore Shape::
get_cylinder(int circle_points, 
             LVecBase3f start_point,
             float start_radius,
             LVecBase3f end_point,
             float end_radius) {
  GeomStore g;
  nassertr(start_radius != 0.0f || end_radius != 0.0f, g);
  nassertr(start_radius >= 0.0f && end_radius >= 0.0f, g);
  nassertr(circle_points > 2, g);
  int num_circles;
  LVecBase3f direction = end_point - start_point;
  float length = direction.length();
  float seg_len;
  seg_len = PI * start_radius + PI * end_radius;
  seg_len /= (float) circle_points;
  num_circles = max((int) round(length / seg_len) + 1, 2);
  g.set_num_rows((num_circles + 2) * circle_points + 2);

  /* Insert Vertices */  
  vector<vector<int>> v_id;
  if (start_radius > 0.0f) {
    vector<int> t = {g.add_vertex(start_point)};
    v_id.push_back(t);
  }
  for (int i = 0; i < num_circles; i++) {
    float part = (float) i / (float) (num_circles - 1);
    float r = (1.0f - part) * start_radius;
    r += part * end_radius;
    LVecBase3f center = direction * ((float) i / (float) (num_circles - 1));
    center += start_point;
    vector<int> row_id;
    if (r == 0.0f) {
      row_id.push_back(g.add_vertex(center));
    }
    else {
      PTA_LVecBase3f circle = get_circle(
        circle_points, 
        r, 
        direction);
      row_id.reserve(circle.size());
      for (auto it : circle) {
        row_id.push_back(g.add_vertex(it + center));
      }
    }
    v_id.push_back(row_id);
  } 
  if (end_radius > 0.0f) {
    vector<int> t = {g.add_vertex(end_point)};
    v_id.push_back(t);
  }
  
  /* Connect Vertices with triangles */
  for (int i = 0; i < v_id.size() - 1; i++) {
    if (v_id[i].size() == v_id[i + 1].size()) {
      for (int j = 0; j < v_id[i].size(); j++) {
        int v0 = v_id[i][j];
        int v1 = v_id[i][(j + 1) % v_id[i].size()];
        int v2 = v_id[i + 1][(j + 1) % v_id[i + 1].size()];
        int v3 = v_id[i + 1][j];
        nassertr(g.add_quad(v0, v1, v2, v3) != -1, g);
      }
    }
    else if (v_id[i].size() == 1) {
      int v0 = v_id[i][0];
      for (int j = 0; j < v_id[i + 1].size(); j++) {
        int v1 = v_id[i + 1][(j + 1) % v_id[i + 1].size()];
        int v2 = v_id[i + 1][j];
        nassertr(g.add_triangle(v0, v1, v2) != -1, g);
      }
    }
    else {
      int v2 = v_id[i + 1][0];
      for (int j = 0; j < v_id[i].size(); j++) {
        int v0 = v_id[i][j];
        int v1 = v_id[i][(j + 1) % v_id[i].size()];
        nassertr(g.add_triangle(v0, v1, v2) != -1, g);
      }
    }
  }

  return g;
}


/**
 * Returns a PTA_LVecBase3f of `segments` length, containing the coordinates
 * of a circle of `radius` (default = 1.0f) with `direction` as its up vector.
 */
PTA_LVecBase3f Shape::
get_circle(int segments, float radius, LVecBase3f direction) {
  PTA_LVecBase3f p_pta;
  p_pta.reserve(segments);
  float rad_step = TWOPI / (float) segments;
  direction.normalize();
  bool rot = (direction == LVecBase3f(0.0f, 0.0f, 1.0f)) ? false : true;

  LQuaternionf q;
  if (rot) {
    LVecBase3f v1 = {0.0f, 0.0f, 1.0f};
    LVecBase3f a = v1.cross(direction);
    q[1] = a[0];
    q[2] = a[1];
    q[3] = a[2];
    q[0] = 1.0f + v1.dot(direction);
    q.normalize();
  }

  for (int i = 0; i < segments; i++) {
    p_pta.push_back(LVecBase3f(
      cosf(i * rad_step) * radius, 
      sinf(i * rad_step) * radius, 
      0.0f));
    if (rot) {
      p_pta[i] = q.xform(p_pta[i]);
    }
  }
  
  return p_pta;
}