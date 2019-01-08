import panda3d.core
from panda3d.core import Vec3, Vec4
import geomstore
import numpy as np

g = geomstore.GeomStore()
v0 = g.add_vertex(Vec3(-2, 0, 0), Vec4(1.0))
v1 = g.add_vertex(Vec3(2, 0, 0), Vec4(1.0))
v2 = g.add_vertex(Vec3(2, 0, 1), Vec4(1.0))
v3 = g.add_vertex((-2., 0., 1.), Vec4(1.0))
g.add_triangle(v0, v1, v2)
g.add_triangle(v2, v3, v0)
v0 = g.add_vertex(Vec3(-2, 0, 1), Vec4(0.8))
v1 = g.add_vertex(Vec3(2, 0, 1), Vec4(0.8))
v2 = g.add_vertex(Vec3(2, 1, 1), Vec4(0.8))
v3 = g.add_vertex((-2., 1., 1.), Vec4(0.8))
g.add_triangle(v0, v1, v2)
g.add_triangle(v2, v3, v0)
v0 = g.add_vertex(Vec3(-2, 1, 1), Vec4(0.7))
v1 = g.add_vertex(Vec3(2, 1, 1), Vec4(0.7))
v2 = g.add_vertex(Vec3(2, 0, 1), Vec4(0.7))
v3 = g.add_vertex((-2., 0., 1.), Vec4(0.7))
g.add_triangle(v0, v1, v2)
g.add_triangle(v2, v3, v0)
v0 = g.add_vertex(Vec3(2, 1, 0), Vec4(0.6))
v1 = g.add_vertex(Vec3(-2, 1, 0), Vec4(0.6))
v2 = g.add_vertex(Vec3(-2, 1, 1), Vec4(0.6))
v3 = g.add_vertex((2., 1., 1.), Vec4(0.6))
g.add_triangle(v0, v1, v2)
g.add_triangle(v2, v3, v0)
v0 = g.add_vertex(Vec3(2, 0, 0), Vec4(0.4))
v1 = g.add_vertex(Vec3(2, 1, 0), Vec4(0.4))
v2 = g.add_vertex(Vec3(2, 1, 1), Vec4(0.4))
v3 = g.add_vertex((2, 0., 1.), Vec4(0.4))
g.add_triangle(v0, v1, v2)
g.add_triangle(v2, v3, v0)
v0 = g.add_vertex(Vec3(-2, 1, 0), Vec4(0.2))
v1 = g.add_vertex(Vec3(-2, 0, 0), Vec4(0.2))
v2 = g.add_vertex(Vec3(-2, 0, 1), Vec4(0.2))
v3 = g.add_vertex((-2, 1., 1.), Vec4(0.2))
g.add_triangle(v0, v1, v2)
g.add_triangle(v2, v3, v0)

print('before subdivide')
print('----------------')
print('vertices:')
g.print_vertices()
print('colors:')
g.print_colors()
print('triangles:')
g.print_triangles()
print('\n')

g.subdivide_triangles(1)

print('after subdivide')
print('---------------')
print('vertices:')
g.print_vertices()
print('colors:')
g.print_colors()
print('triangles:')
g.print_triangles()
print('\n')

a = np.asarray(memoryview(g._vertex_positions))
print(a)

g.subdivide_triangles_distance(0.2)

print('after subdivide_distance')
print('------------------------')
print('vertices:')
g.print_vertices()
print('colors:')
g.print_colors()
print('triangles:')
g.print_triangles()
print('\n')

g * 10.0
g + 5
g / 2
g - 5
g + Vec3(2.0)
g - Vec3(4.0)

print('after operator testing')
print('------------------------')
print('vertices:')
g.print_vertices()
print('\n')

g.set_color(Vec4(1))

print('after color set all')
print('------------------------')
print('colors:')
g.print_colors()
print('\n')

g.set_color(Vec4(0.5), [0, 1, 2])

print('after color set with filter')
print('------------------------')
print('colors:')
g.print_colors()
print('\n')

g.normals_as_color()

print('after normals_as_color')
print('------------------------')
print('colors:')
g.print_colors()
print('\n')

g.to_unit_sphere()

print('after to_unit_sphere')
print('------------------------')
print('vertices:')
g.print_vertices()
print('\n')

n = g.get_p3d_geom_node('Test')

print(n)