"""
Provides a convenient form to store geometry, have it "magically" calculate
 face and vertex normals for you and export it as GeomNode objects.
Uses flat shading, because I liked the look of it. Automatically duplicates
 vertices to get single colors and normals (face + vertex) per triangle.
"""
from typing import Dict
from typing import NamedTuple
from typing import Optional
from typing import Union

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
        v_id = self.__get_vertex_id_by_obj__(vertex)
        if v_id:
            return v_id
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
        t_id = self.__get_triangle_id_by_obj__(triangle)
        if t_id:
            return t_id
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
