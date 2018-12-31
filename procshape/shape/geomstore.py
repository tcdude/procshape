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

from panda3d.core import Geom
from panda3d.core import GeomNode
from panda3d.core import GeomTriangles
from panda3d.core import GeomVertexData
from panda3d.core import GeomVertexFormat
from panda3d.core import GeomVertexWriter
from panda3d.core import LVector3f

from procshape.helpers.shapetools import get_vertex_id
from procshape.helpers.shapetools import subdivide_triangles
from procshape.helpers.shapetools import subdivide_triangles_dist
from procshape.helpers.types import C
from procshape.helpers.types import NPA
from procshape.helpers.types import V3
from procshape.helpers.types import V4
from procshape.helpers.vectormath import triangle_face_normal


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
     the fly, since there are no normals stored. See https://bit.ly/2QOlfbh
    """

    def __init__(self):
        self.__vertices__ = None
        self.__colors__ = None
        self.__triangles__ = None
        self.__face_normals__ = None
        self.__edges__ = None
        self.__costs__ = None
        self.__lowest_dirty_triangle__ = -1

    @property
    def vertices(self):
        return self.__vertices__

    @property
    def triangles(self):
        return self.__triangles__

    @property
    def colors(self):
        return self.__colors__

    @property
    def face_normals(self):
        return self.__face_normals__

    @property
    def edges(self):
        return self.__edges__

    def update_face_normals(self):
        """
        Updates all face normals for triangles with id >=
        self.__lowest_dirty_triangle__
        """
        if self.face_normals is None:
            self.__face_normals__ = triangle_face_normal(
                self.vertices[self.triangles[..., 0]],
                self.vertices[self.triangles[..., 1]],
                self.vertices[self.triangles[..., 2]],
                True
            )
            self.__lowest_dirty_triangle__ = len(self.triangles)
            return
        face_normals = np.empty((len(self.triangles), 3), dtype=np.float32)
        l_id = self.__lowest_dirty_triangle__
        face_normals[:l_id] = self.face_normals[:l_id]
        face_normals[l_id:] = triangle_face_normal(
            self.vertices[self.triangles[l_id:, 0]],
            self.vertices[self.triangles[l_id:, 1]],
            self.vertices[self.triangles[l_id:, 2]],
            True
        )
        self.__face_normals__ = face_normals

    def deduplicate_vertices(self):
        """Removes duplicate vertices in the GeomStore"""
        combined = np.empty((len(self.vertices), 7), dtype=np.float32)
        combined[:, :3] = self.vertices
        combined[:, -4:] = self.colors
        arr, ids = np.unique(combined, return_inverse=True, axis=0)
        self.__vertices__ = arr[:, :3]
        self.__colors__ = arr[:, -4:]
        self.__triangles__ = ids[self.triangles]

    def reduce_triangles(self, factor=0.5, bounding_box=None, origin=None):
        # type: (Optional[float], Optional[V3], Optional[V3]) -> None
        """
        Reduce triangle count in GeomStore. Optionally can be constrained by
        a ``bounding_box`` and if needed, offsetting the ``origin`` of the
        ``bounding_box``. In this case, only triangles that lie entirely inside
        the specified ``bounding_box`` (and not on the border) will be
        considered for the reduction.

        Arguments:
            factor: removes triangles until ceil(``factor`` * triangle_count)
                triangles remain. Must be in range 0 < ``factor`` < 1
            bounding_box: If passed, only triangles within the specified
                positive/negative half distance will be reduced. If any of the
                axes is set to 0, the entire bounds of that axis will be
                included.
            origin: If passed, serves as origin for the ``bounding_box`` arg
        """
        if origin is not None and bounding_box is None:
            raise ValueError('Argument origin must be set in combination with '
                             'the bounding_box argument!')
        if not (0 < factor < 1):
            raise ValueError('Argument factor must meet: 0 < factor < 1')
        verts = self.vertices
        if bounding_box is None:
            tri_ids = list(range(len(self.triangles)))
        else:
            bb = []
            for i, v in enumerate(bounding_box):
                if v == 0:
                    bb.append(max(
                        verts[:, i].max(),
                        abs(verts[:, i].min())
                    ))
            bb = np.array(bb, dtype=np.float32)
            bb_min = -bb
            bb_max = bb
            if origin is not None or origin != (0, 0, 0):
                bb_min += np.array(origin, dtype=np.float32)
                bb_max += np.array(origin, dtype=np.float32)
            tri_ids = np.unique(
                np.where(
                    (bb_min < verts[self.triangles]).all(axis=1) &
                    (bb_max > verts[self.triangles]).all(axis=1)
                )
            )
        triangles = self.triangles.copy()[tri_ids]
        # Find Minimum Cost Collapse for each Edge

        # Store a sequence to reduce the triangles, keeping the vertices intact
        # Keep sequence in reverse to enable restoration of prior state

    def to_unit_sphere(self):
        """Sets every vertex to unit length with origin (0, 0, 0)"""
        self.__vertices__ = self.get_vertex_normals()

    def get_vertex_normals(self):
        """Returns vertices normalized to unit length with origin (0, 0, 0)"""
        lengths = np.sqrt(
            (self.vertices ** 2).sum(
                axis=1
            ).reshape(self.vertices.shape[0], 1)
        )
        return self.vertices / lengths

    def __mul__(self, other):
        # type: (Union[float, NPA, LVector3f]) -> bool
        """
        Multiplies the vertices by either scalar float, ndarray of shape (3,)
        or panda3d.core.LVector3f
        """
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

    def __add__(self, other):
        # type: (Union[float, NPA, LVector3f]) -> bool
        """
        Add either scalar float, ndarray of shape (3,) or panda3d.core.LVector3f
        to the vertices.
        """
        if isinstance(other, float):
            self.__vertices__ += other
            return True
        elif isinstance(other, NPA):
            if other.shape == (3,):
                self.__vertices__ += other
                return True
        else:
            try:
                self.__vertices__ += np.array(other, dtype=np.float32)
            except ValueError:
                pass
            else:
                return True
        raise ValueError('Expected scalar float, panda3d Vec3 or ndarray '
                         f'of shape (3, ), got {type(other)} instead.')

    def __sub__(self, other):
        # type: (Union[float, NPA, LVector3f]) -> bool
        """
        Subtract either scalar float, ndarray of shape (3,) or
        panda3d.core.LVector3f from the vertices.
        """
        if isinstance(other, float):
            self.__vertices__ -= other
            return True
        elif isinstance(other, NPA):
            if other.shape == (3,):
                self.__vertices__ -= other
                return True
        else:
            try:
                self.__vertices__ -= np.array(other, dtype=np.float32)
            except ValueError:
                pass
            else:
                return True
        raise ValueError('Expected scalar float, panda3d Vec3 or ndarray '
                         f'of shape (3, ), got {type(other)} instead.')

    def __truediv__(self, other):
        # type: (Union[float, NPA, LVector3f]) -> bool
        """
        Divides vertices by either scalar float, ndarray of shape (3,) or
        panda3d.core.LVector3f.
        """
        if isinstance(other, float):
            self.__vertices__ /= other
            return True
        elif isinstance(other, NPA):
            if other.shape == (3,):
                self.__vertices__ /= other
                return True
        else:
            try:
                self.__vertices__ /= np.array(other, dtype=np.float32)
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
        """Sets ``color`` for all vertices, or a ``selection`` of vertices"""
        ids = selection or range(len(self.colors))
        for i in ids:
            self.__colors__[i] = color

    def normals_as_colors(self):
        """Uses unit length from origin (0, 0, 0) as vertex color"""
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
        """Subdivides mesh until triangle hypotenuse < ``target_distance``"""
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
            self.__lowest_dirty_triangle__ = 0
            return 0
        idx = len(self.__triangles__)
        self.__triangles__ = np.append(
            self.__triangles__,
            [[v1_id, v2_id, v3_id]],
            0
        )
        self.__lowest_dirty_triangle__ = idx
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
