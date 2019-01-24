/**
 * @file csg.h
 * @author Tiziano Bettio
 * @date 2019-01-20
 */

#ifndef CSG_H
#define CSG_H

#include "luse.h"

struct Triangle {
  LVecBase3f V0;
  LVecBase3f V1;
  LVecBase3f V2;
};

struct Ray {
  LVecBase3f P0;
  LVecBase3f P1;
};

int intersect3D_RayTriangle( Ray R, Triangle T, LVecBase3f* I );

#endif