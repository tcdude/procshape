"""
Provides a convenient form to store geometry, subdivide triangles and export
it as GeomNode objects.
Uses flat shading, because I liked the look of it. Automatically duplicates
 vertices to get single colors per triangle.
"""
from typing import List
from typing import Optional
from typing import Tuple
from typing import Union

import numpy as np
from numba import jit

from panda3d.core import Geom
from panda3d.core import GeomNode
from panda3d.core import GeomTriangles
from panda3d.core import GeomVertexData
from panda3d.core import GeomVertexFormat
from panda3d.core import GeomVertexWriter
from panda3d.core import LVector3f

from procshape.helpers.types import C
from procshape.helpers.types import NPA
from procshape.helpers.types import V3
from procshape.helpers.types import V4


def get_vertex_id(point, color, vertices, colors):
    # type: (V3, V4, NPA, NPA) -> int
    """Returns matching array index or -1 if none was found"""
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
def subdivide_triangles(vertices, triangles, colors, subdivisions=1):
    # type: (NPA, NPA, NPA, int) -> Tuple[NPA, NPA, NPA]
    """Subdivides every triangle on its longest side ``subdivisions`` times"""
    distance_vec = (vertices[np.roll(triangles, -1, 1)] - vertices[triangles])
    indices = (distance_vec ** 2).sum(axis=2).argmax(axis=1)
    assert(len(distance_vec) == len(indices))
    old_tri_len = len(triangles)
    new_triangles = np.empty((old_tri_len * 2, 3), dtype=np.int64)
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
    if subdivisions > 1:
        return subdivide_triangles(
            new_vertices,
            new_triangles,
            new_colors,
            subdivisions - 1
        )
    else:
        return new_vertices, new_triangles, new_colors


def subdivide_triangles_dist(vertices, triangles, colors, target_distance=2.0):
    # type: (NPA, NPA, NPA, float) -> Tuple[NPA, NPA, NPA]
    _check_dist = target_distance ** 2
    num_new_rows = 1
    while num_new_rows:
        if triangles.dtype != np.int64:
            print(triangles.dtype, len(triangles))
        dist_vec = (vertices[np.roll(triangles, -1, 1)] - vertices[triangles])
        len_vec = (dist_vec ** 2).sum(axis=2)
        vec_max = len_vec.max(axis=1)
        rows = np.where(vec_max > _check_dist)[0]
        num_new_rows = len(rows)
        if num_new_rows == 0:
            break
        indices = len_vec[rows].argmax(axis=1)
        colors, triangles, vertices = _subdivide(vertices, triangles, colors,
                                                 indices, rows, dist_vec,
                                                 num_new_rows)

    return vertices, triangles, colors


@jit(nopython=True)
def _subdivide(vertices, triangles, colors, indices, rows, distance_vec,
               num_new_rows):
    # type: (NPA, NPA, NPA, NPA, NPA, NPA, int) -> Tuple[NPA, NPA, NPA]
    old_tri_len = len(triangles)
    new_triangles = np.empty(
        (old_tri_len + num_new_rows, 3),
        dtype=np.int64
    )
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
    return colors, triangles, vertices


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

    def to_unit_sphere(self):
        self.__vertices__ = self.get_vertex_normals()

    def get_vertex_normals(self):
        lengths = np.sqrt(
            (self.vertices ** 2).sum(
                axis=1
            ).reshape(self.vertices.shape[0], 1)
        )
        return self.vertices / lengths

    def __mul__(self, other):
        # type: (Union[float, NPA, LVector3f]) -> bool
        if isinstance(other, float):
            self.__vertices__ *= other
            return True
        elif isinstance(other, NPA):
            if other.shape == (3,):
                self.__vertices__ *= other
                return True
        else:
            try:
                self.__vertices__ *= np.array(other, dtype=np.float32)
            except ValueError:
                pass
            else:
                return True
        raise ValueError('Expected scalar float, panda3d Vec3 or ndarray '
                         f'of shape (3, ), got {type(other)} instead.')

    def extend(self, other):
        # type: (GeomStore) -> None
        """
        Extends this GeomStore with ``other``. Does not prevent duplicates
         of any kind.
        """
        if self.vertices is None:
            self.__vertices__ = other.vertices
            self.__triangles__ = other.triangles
            self.__colors__ = other.colors
            return
        this_verts = len(self.vertices)
        other_verts = len(other.vertices)
        other_tris = len(other.triangles)
        if 0 in (other_verts, other_tris):
            raise ValueError('Cannot extend: No vertices and/or '
                             'triangles found in other GeomStore object.')

        self.__vertices__ = np.append(self.vertices, other.vertices, axis=0)
        self.__colors__ = np.append(self.colors, other.colors, axis=0)
        self.__triangles__ = np.append(
            self.triangles,
            other.triangles + this_verts,
            axis=0
        )

    def set_color(self, color, selection=None):
        # type: (V4, Optional[Union[List[int, ...], Tuple[int, ...]]]) -> None
        ids = selection or range(len(self.colors))
        for i in ids:
            self.__colors__[i] = color

    def normals_as_colors(self):
        normals = self.get_vertex_normals()
        colors = np.empty((len(normals), 4), dtype=np.float32)
        colors[:, :3] = normals * 0.5 + 0.5
        colors[:, -1:] = 1.0
        self.__colors__ = colors

    def subdivide_mesh(self, subdivisions=1):
        # type: (Optional[int]) -> None
        """Subdivides mesh ``subdivisions`` times"""
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
        # type: (Optional[float]) -> None
        """Subdivides mesh until triangle side length < ``target_distance``"""
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
                dtype=np.int64
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
        """Returns a Panda3D GeomNode object"""
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
