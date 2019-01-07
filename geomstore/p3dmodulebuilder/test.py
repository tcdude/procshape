import panda3d.core
from panda3d.core import Vec3, Vec4
import geomstore
import numpy as np

g = geomstore.GeomStore()
v0 = g.add_vertex(Vec3(-2, 0, 0), Vec4(1.0))
v1 = g.add_vertex(Vec3(2, 0, 0), Vec4(0.5))
v2 = g.add_vertex(Vec3(2, 0, 1), Vec4(0.0))
v3 = g.add_vertex(Vec3(-2, 0, 1), Vec4(0.0))
g.add_triangle(v0, v1, v2)
g.add_triangle(v2, v3, v0)

print('before subdivide')
print('vertices:')
g.print_vertices()
print('colors:')
g.print_colors()
print('triangles:')
g.print_triangles()

g.subdivide_triangles(1)
g.subdivide_triangles_distance(0.2)

print('after subdivide')
print('vertices:')
g.print_vertices()
print('colors:')
g.print_colors()
print('triangles:')
g.print_triangles()
