/**
 * @file shape.h
 * @author Tiziano Bettio
 * @date 2019-01-10
 */

#ifndef SHAPE_HPP
#define SHAPE_HPP


#include <iostream>
#include <cmath>

#include "pandabase.h"
#include "luse.h"
#include "nodePath.h"

#include "config_module.h"
#include "geomstore.h"
#include "common.h"

void set_dimensions(LVecBase3f, int, int, int);

/**
 * Provides a few basic 3D shapes.
 */
class Shape {
  PUBLISHED:
    Shape();
    ~Shape();
    GeomStore get_plane(
      LVecBase3f origin, 
      LVecBase2f bounds, 
      LVecBase3f normal, 
      bool two_sided = false);
    GeomStore get_cuboid(LVecBase3f bounding_box, LVecBase3f origin_offset);
    GeomStore get_spheroid(LVecBase3f bounding_box, LVecBase3f origin_offset);
    GeomStore get_spheroid_exp(LVecBase3f bounding_box, LVecBase3f origin_offset);
    GeomStore get_cylinder(
      int circle_points, 
      LVecBase3f start_point,
      float start_radius,
      LVecBase3f end_point,
      float end_radius);
    PTA_LVecBase3f get_circle(
      int segments,
      float radius = 1.0f,
      LVecBase3f direction = LVecBase3f(0.0f, 0.0f, 1.0f));
};

#endif