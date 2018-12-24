"""
Provides a convenient form to store geometry, have it "magically" calculate
 face and vertex normals for you and export it as GeomNode objects.
Uses flat shading, because I liked the look of it. Automatically duplicates
 vertices to get single colors and normals (face + vertex) per triangle.
"""
from typing import Dict
from typing import NamedTuple
from typing import Optional
from typing import Tuple
from typing import Union

import numpy as np
from numba import float32
from numba import int32
from numba import jit

from panda3d.core import Geom
from panda3d.core import GeomNode
from panda3d.core import GeomTriangles
from panda3d.core import GeomVertexData
from panda3d.core import GeomVertexFormat
from panda3d.core import GeomVertexWriter
from panda3d.core import Vec3
from panda3d.core import Vec4


class Vertex(NamedTuple):
    point: Vec3
    color: Union[Vec3, Vec4]


class Triangle(NamedTuple):
    v1: Vertex
    v2: Vertex
    v3: Vertex


V3 = Tuple[float, float, float]
V4 = Tuple[float, float, float, float]
C = Union[V3, V4]


# @jit(nopython=True)
def get_vertex_id(point, color, vertices, colors):
    for i in range(len(vertices)):
        res = True
        for k in range(4):
            if k < 3:
                if point[k] != vertices[i][k] or color[k] != colors[i][k]:
                    res = False
                    break
            else:
                if color[k] != colors[i][k]:
                    res = False
                    break
        if res:
            return i
    return -1


# @jit(parallel=True)
def subdivide_triangles(vertices, triangles, colors, subdivisions=2):
    distance_vec = (vertices[np.roll(triangles, -1, 1)] - vertices[triangles])
    indices = (distance_vec ** 2).sum(axis=2).argmax(axis=1)
    assert(len(distance_vec) == len(indices))
    old_tri_len = len(triangles)
    new_triangles = np.empty((old_tri_len * 2, 3), dtype=np.uint32)
    new_triangles[:old_tri_len] = triangles.copy()
    old_vert_len = len(vertices)
    new_vertices = np.empty(
        (old_vert_len + old_tri_len, 3),
        dtype=np.float32
    )
    new_vertices[:old_vert_len] = vertices.copy()
    new_colors = np.empty(
        (old_vert_len + old_tri_len, 4),
        dtype=np.float32
    )
    new_colors[:old_vert_len] = colors.copy()
    for i, start_idx in enumerate(indices):
        new_tri = triangles[i].copy()
        dist = distance_vec[i, start_idx] * 0.5
        new_point = dist + vertices[triangles[i, start_idx]]
        new_point_id = old_vert_len + i
        new_vertices[new_point_id] = new_point
        to_idx = (start_idx + 1) % 3
        new_color = new_colors[triangles[i][start_idx]]
        new_color += new_colors[triangles[i][to_idx]]
        new_color *= 0.5
        new_colors[new_point_id] = new_color
        new_triangles[i][to_idx] = new_point_id
        new_tri[start_idx] = new_point_id
        new_triangles[i + old_tri_len] = new_tri
    if subdivisions > 2:
        return subdivide_triangles(
            new_vertices,
            new_triangles,
            new_colors,
            subdivisions - 1
        )
    else:
        return new_vertices, new_triangles, new_colors


def subdivide_triangles_dist(vertices, triangles, colors, target_distance=2.0):
    _check_dist = target_distance ** 2
    new_vertices = None
    new_triangles = None
    new_colors = None
    num_new_rows = 1
    while num_new_rows:
        distance_vec = (vertices[np.roll(triangles, -1, 1)] - vertices[triangles])
        len_vec = (distance_vec ** 2).sum(axis=2)
        vec_max = len_vec.max(axis=1)
        rows = np.where(vec_max > _check_dist)[0]
        num_new_rows = len(rows)
        if num_new_rows == 0:
            break
        indices = len_vec[rows].argmax(axis=1)
        old_tri_len = len(triangles)
        new_triangles = np.empty((old_tri_len + num_new_rows, 3), dtype=np.uint32)
        new_triangles[:old_tri_len] = triangles.copy()
        old_vert_len = len(vertices)
        new_vertices = np.empty(
            (old_vert_len + num_new_rows, 3),
            dtype=np.float32
        )
        new_vertices[:old_vert_len] = vertices.copy()
        new_colors = np.empty(
            (old_vert_len + num_new_rows, 4),
            dtype=np.float32
        )
        new_colors[:old_vert_len] = colors.copy()
        for i, start_idx in enumerate(indices):
            new_tri = triangles[rows[i]].copy()
            dist = distance_vec[rows[i], start_idx] * 0.5
            new_point = dist + vertices[triangles[rows[i], start_idx]]
            new_point_id = old_vert_len + i
            new_vertices[new_point_id] = new_point
            to_idx = (start_idx + 1) % 3
            new_color = new_colors[triangles[rows[i]][start_idx]]
            new_color += new_colors[triangles[rows[i]][to_idx]]
            new_color *= 0.5
            new_colors[new_point_id] = new_color
            new_triangles[rows[i]][to_idx] = new_point_id
            new_tri[start_idx] = new_point_id
            new_triangles[i + old_tri_len] = new_tri
        vertices = new_vertices
        triangles = new_triangles
        colors = new_colors

    return new_vertices, new_triangles, new_colors


class GeomStore(object):
    """
    Stores Vertex and Triangle objects and provides functionality to export
     into a Panda3D GeomNode object, using GeomVertexFormat.get_v3c4().
    Using RGBA colors to use the A-channel for glossmaps or other shader
     specific input. The export method will automatically calculate the average
     of every vertex color for each triangle. If you want a single alpha value
     per triangle, make sure to set the same value on each vertex passed to the
     GeomStore.add_triangle() method.
    Caution: You need to use a shader that calculates the face normal on
     the fly, since there are no normals calculated. See https://bit.ly/2QOlfbh
    """

    def __init__(self):
        self.__vertices__ = None
        self.__colors__ = None
        self.__triangles__ = None

    @property
    def vertices(self):
        return self.__vertices__

    @property
    def triangles(self):
        return self.__triangles__

    @property
    def colors(self):
        return self.__colors__

    def subdivide_mesh(self, subdivisions=2):
        v, t, c = subdivide_triangles(
            self.vertices,
            self.triangles,
            self.colors,
            subdivisions
        )
        self.__vertices__ = v
        self.__triangles__ = t
        self.__colors__ = c

    def subdivide_mesh_dist(self, target_distance=2.0):
        v, t, c = subdivide_triangles_dist(
            self.vertices,
            self.triangles,
            self.colors,
            target_distance
        )
        self.__vertices__ = v
        self.__triangles__ = t
        self.__colors__ = c

    def add_vertex(self, point, color):
        # type: (V3, C) -> int
        """
        Adds a Vertex to the GeomStore and avoids duplicates.
        Returns vertex id
        """
        if len(color) == 3:
            color += (1.,)
        if self.__vertices__ is None:
            self.__vertices__ = np.array([list(point)], dtype=np.float32)
            self.__colors__ = np.array([list(color)], dtype=np.float32)
            return 0
        idx = get_vertex_id(point, color, self.vertices, self.colors)
        if idx > -1:
            return idx
        idx = len(self.__vertices__)
        self.__vertices__ = np.append(self.__vertices__, [list(point)], 0)
        self.__colors__ = np.append(self.__colors__, [list(color)], 0)
        return idx

    def add_triangle(self, v1_id, v2_id, v3_id):
        # type: (int, int, int) -> int
        """
        Adds a Triangle to the GeomStore from vertex ids. Arguments must be
        passed in CCW order!
        Returns triangle id
        """
        if self.__triangles__ is None:
            self.__triangles__ = np.array(
                [[v1_id, v2_id, v3_id]],
                dtype=np.uint32
            )
            return 0
        idx = len(self.__triangles__)
        self.__triangles__ = np.append(
            self.__triangles__,
            [[v1_id, v2_id, v3_id]],
            0
        )
        return idx

    # noinspection PyArgumentList
    def get_p3d_geom_node(self, name='UnnamedGeom'):
        # type: (Optional[str]) -> GeomNode
        """"""
        vertex_data = GeomVertexData(
            name,
            GeomVertexFormat.get_v3c4(),
            Geom.UH_static
        )
        total_triangles = len(self.__triangles__)

        # Every triangle gets its unique set of vertices for flat shading
        num_rows = total_triangles * 3
        vertex_data.set_num_rows(num_rows)
        vertex_writer = GeomVertexWriter(vertex_data, 'vertex')
        color_writer = GeomVertexWriter(vertex_data, 'color')
        for i in range(total_triangles):
            triangle = self.vertices[self.triangles[i]]
            vertex_writer.add_data3f(*triangle[0])
            vertex_writer.add_data3f(*triangle[1])
            vertex_writer.add_data3f(*triangle[2])
            triangle_color = self.colors[self.triangles[i]].sum(axis=0) / 3
            for _ in [0, 0, 0]:
                color_writer.add_data4f(*triangle_color)

        geom = Geom(vertex_data)
        for i in range(total_triangles):
            triangle = GeomTriangles(Geom.UH_static)
            first = i * 3
            triangle.add_vertex(first)
            triangle.add_vertex(first + 1)
            triangle.add_vertex(first + 2)
            geom.add_primitive(triangle)
        node = GeomNode(name)
        node.add_geom(geom)
        return node


class GeomStoreOld(object):
    """
    Stores Vertex and Triangle objects and provides functionality to export
     into a Panda3D GeomNode object, using GeomVertexFormat.get_v3c4().
    Using RGBA colors to use the A-channel for glossmaps or other shader
     specific input. The export method will automatically calculate the average
     of every vertex color for each triangle. If you want a single alpha value
     per triangle, make sure to set the same value on each vertex passed to the
     GeomStore.add_triangle() method.
    Caution: You need to use a shader that calculates the face normal on
     the fly, since there are no normals calculated. See https://bit.ly/2QOlfbh
    """

    def __init__(self):
        self.__vertices__ = {}  # type: Dict[int, Vertex]
        self.__triangles__ = {}  # type: Dict[int, Triangle]
        self.__vertex_id__ = 0
        self.__triangle_id__ = 0

    # noinspection PyArgumentList
    def get_p3d_geom_node(self, name='UnnamedGeom'):
        # type: (Optional[str]) -> GeomNode
        """"""
        vertex_data = GeomVertexData(
            name,
            GeomVertexFormat.get_v3c4(),
            Geom.UH_static
        )
        total_triangles = len(self.__triangles__)

        # Every triangle gets its unique set of vertices for flat shading
        num_rows = total_triangles * 3
        vertex_data.set_num_rows(num_rows)
        vertex_writer = GeomVertexWriter(vertex_data, 'vertex')
        color_writer = GeomVertexWriter(vertex_data, 'color')
        for v1, v2, v3 in self.__triangles__.values():
            triangle_color = (v1.color + v2.color + v3.color) / 3
            if len(triangle_color) == 3:
                triangle_color = Vec4(*tuple(triangle_color), 1.0)
            vertex_writer.add_data3f(v1.point)
            vertex_writer.add_data3f(v2.point)
            vertex_writer.add_data3f(v3.point)
            for _ in [0, 0, 0]:
                color_writer.add_data4f(triangle_color)

        geom = Geom(vertex_data)
        for i in range(total_triangles):
            triangle = GeomTriangles(Geom.UH_static)
            first = i * 3
            triangle.add_vertex(first)
            triangle.add_vertex(first + 1)
            triangle.add_vertex(first + 2)
            geom.add_primitive(triangle)
        node = GeomNode(name)
        node.add_geom(geom)
        return node

    def add_triangle_from_points(self, p1, p2, p3, color):
        # type: (Vec3, Vec3, Vec3, Union[Vec3, Vec4]) -> int
        v1_id = self.add_vertex(p1, color)
        v2_id = self.add_vertex(p2, color)
        v3_id = self.add_vertex(p3, color)
        return self.add_triangle(v1_id, v2_id, v3_id)

    def add_vertex(self, point, color):
        # type: (Vec3, Union[Vec3, Vec4]) -> int
        """
        Adds a Vertex to the GeomStore and avoids duplicates.
        Returns vertex id
        """
        vertex = Vertex(point=point, color=color)
        # v_id = self.__get_vertex_id_by_obj__(vertex)
        # if v_id:
        #     return v_id
        self.__vertex_id__ += 1
        self.__vertices__[self.__vertex_id__] = vertex
        return self.__vertex_id__

    def add_triangle(self, v1_id, v2_id, v3_id):
        # type: (int, int, int) -> int
        """
        Adds a Triangle to the GeomStore from vertex ids. Arguments must be
        passed in CCW order!
        Returns triangle id
        """
        triangle = Triangle(
            v1=self.__get_vertex_by_id__(v1_id),
            v2=self.__get_vertex_by_id__(v2_id),
            v3=self.__get_vertex_by_id__(v3_id),
        )
        # t_id = self.__get_triangle_id_by_obj__(triangle)
        # if t_id:
        #     return t_id
        self.__triangle_id__ += 1
        self.__triangles__[self.__triangle_id__] = triangle
        return self.__triangle_id__

    def __get_vertex_by_id__(self, v_id):
        """Returns Vertex stored under v_id"""
        if v_id in self.__vertices__:
            return self.__vertices__[v_id]
        raise ValueError(f'No Vertex with id={v_id} found!')

    def __get_vertex_id_by_obj__(self, vertex):
        # type: (Vertex) -> int
        """If the same vertex already exists, returns the id, else 0"""
        for vertex_id, _vertex in self.__vertices__.items():
            if vertex == _vertex:
                return vertex_id
        return 0

    def __get_triangle_by_id__(self, t_id):
        """Returns Triangle stored under t_id"""
        if t_id in self.__triangles__:
            return self.__triangles__[t_id]
        raise ValueError(f'No Triangle with id={t_id} found!')

    def __get_triangle_id_by_obj__(self, triangle):
        # type: (Triangle) -> int
        """If the same triangle already exists, returns the id, else 0"""
        for triangle_id, _triangle in self.__triangles__.items():
            if triangle == _triangle:
                return triangle_id
        return 0
