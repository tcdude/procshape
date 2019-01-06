import panda3d.core
from panda3d.core import Vec3, Vec4
import geomstore
import numpy as np

g = geomstore.GeomStore()

v0 = g.add_vertex(Vec3(1.0), Vec4(1.0))
v1 = g.add_vertex(Vec3(0.5), Vec4(0.5))
v2 = g.add_vertex(Vec3(0.0), Vec4(0.0))

g.add_triangle(v0, v1, v2)

print('_vertex_positions...')
g.print_vertices()
print('trying to make an ndarray from _vertex_positions...')
a = np.asarray(memoryview(g._vertex_positions))
print('numpy _vertex_positions...')
print(a)
a[0, 0] = 5.1
print('numpy _vertex_positions modified...')
print(a)
print('_vertex_positions modified...')
g.print_vertices()

print('_colors...')
g.print_colors()
print('trying to make an ndarray from _colors...')
a = np.asarray(memoryview(g._colors))
print('numpy _colors...')
print(a)
a[0, 0] = 5.1
print('numpy _colors modified...')
print(a)
print('_colors modified...')
g.print_colors()

print('_triangle_indices...')
g.print_triangles()
print('trying to make an ndarray from _triangle_indices...')
a = np.asarray(memoryview(g._triangle_indices))
print('numpy _triangle_indices...')
print(a)
a[0, 0] = 5
print('numpy _triangle_indices modified...')
print(a)
print('_triangle_indices modified...')
g.print_triangles()