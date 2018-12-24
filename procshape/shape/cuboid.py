"""
Modular cuboid shape created procedurally inside the extents of a bounding box.
"""
from typing import Optional

from panda3d.core import Vec3

from procshape.shape.geomstore import GeomStore


class Cuboid(object):
    """
    A simple cuboid shape to use as basis for subclassing shapes based on
    cuboids.

    Arguments:
        bounding_box: Vec3 half-extent of the shape
        edge_points: int >= 2, defines how many points there should be per edge
        origin_offset: Vec3, specifies the offset of the origin
    """
    def __init__(self, bounding_box, edge_points=2, origin_offset=Vec3(0)):
        # type: (Vec3, Optional[int], Optional[Vec3]) -> None
        """Minimal setup requirements for a cuboid shape."""
        self.__bounding_box__ = bounding_box
        if not isinstance(edge_points, int) or edge_points < 2:
            raise ValueError('Argument edge_points needs to be a positive int '
                             f'>= 2! Got {type(edge_points)}: {edge_points} '
                             f'instead')
        self.__edge_points__ = edge_points
        self.__origin_offset__ = origin_offset
        self.__geom_store__ = GeomStore()

    @property
    def bounding_box(self):
        return self.__bounding_box__

    @property
    def geom_store(self):
        return self.__geom_store__

    @property
    def edge_points(self):
        return self.__edge_points__

    @property
    def origin_offset(self):
        return self.__origin_offset__

    @property
    def geom_node(self):
        return self.geom_store.get_p3d_geom_node('cuboid')

    def subdivide(self, subdivisions=2):
        self.geom_store.subdivide_mesh(subdivisions)

    def subdivide_dist(self, target_distance=2):
        self.geom_store.subdivide_mesh_dist(target_distance)

    def generate(self):
        edge_divide = self.edge_points - 1
        x_step = self.bounding_box.x * 2 / edge_divide
        y_step = self.bounding_box.y * 2 / edge_divide
        z_step = self.bounding_box.z * 2 / edge_divide

        # Front / Back
        ccw = True
        for y in [
            -self.bounding_box.y + self.origin_offset.y,
            self.bounding_box.y + self.origin_offset.y,
        ]:
            z_start = self.bounding_box.z + self.origin_offset.z
            x_start = -self.bounding_box.x + self.origin_offset.x
            for z in range(edge_divide):
                z_top = z_start - z * z_step
                z_bottom = z_start - (z + 1) * z_step
                for x in range(edge_divide):
                    x_left = x_start + x * x_step
                    x_right = x_start + (x + 1) * x_step
                    v1 = self.geom_store.add_vertex(
                        (x_left, y, z_top),
                        (1., 0., 0.)
                    )
                    v2 = self.geom_store.add_vertex(
                        (x_left, y, z_bottom),
                        (1., 0., 0.)
                    )
                    v3 = self.geom_store.add_vertex(
                        (x_right, y, z_bottom),
                        (1., 0., 0.)
                    )
                    v4 = self.geom_store.add_vertex(
                        (x_right, y, z_top),
                        (1., 0., 0.)
                    )
                    if ccw:
                        self.geom_store.add_triangle(v1, v2, v3)
                        self.geom_store.add_triangle(v3, v4, v1)
                    else:
                        self.geom_store.add_triangle(v1, v4, v3)
                        self.geom_store.add_triangle(v3, v2, v1)
            ccw = False

        # Top / Bottom
        ccw = True
        for z in [
            self.bounding_box.z + self.origin_offset.z,
            -self.bounding_box.z + self.origin_offset.z,
        ]:
            y_start = self.bounding_box.y + self.origin_offset.y
            x_start = -self.bounding_box.x + self.origin_offset.x
            for y in range(edge_divide):
                y_top = y_start - y * y_step
                y_bottom = y_start - (y + 1) * y_step
                for x in range(edge_divide):
                    x_left = x_start + x * x_step
                    x_right = x_start + (x + 1) * x_step
                    v1 = self.geom_store.add_vertex(
                        (x_left, y_top, z),
                        (0., 1., 0.)
                    )
                    v2 = self.geom_store.add_vertex(
                        (x_left, y_bottom, z),
                        (0., 1., 0.)
                    )
                    v3 = self.geom_store.add_vertex(
                        (x_right, y_bottom, z),
                        (0., 1., 0.)
                    )
                    v4 = self.geom_store.add_vertex(
                        (x_right, y_top, z),
                        (0., 1., 0.)
                    )
                    if ccw:
                        self.geom_store.add_triangle(v1, v2, v3)
                        self.geom_store.add_triangle(v3, v4, v1)
                    else:
                        self.geom_store.add_triangle(v1, v4, v3)
                        self.geom_store.add_triangle(v3, v2, v1)
            ccw = False

        # Left / Right
        ccw = True
        for x in [
            -self.bounding_box.x + self.origin_offset.x,
            self.bounding_box.x + self.origin_offset.x,
        ]:
            y_start = self.bounding_box.y + self.origin_offset.y
            z_start = self.bounding_box.z + self.origin_offset.z
            for z in range(edge_divide):
                z_top = z_start - z * z_step
                z_bottom = z_start - (z + 1) * z_step
                for y in range(edge_divide):
                    y_left = y_start - y * y_step
                    y_right = y_start - (y + 1) * y_step
                    v1 = self.geom_store.add_vertex(
                        (x, y_left, z_top),
                        (0., 0., 1.)
                    )
                    v2 = self.geom_store.add_vertex(
                        (x, y_left, z_bottom),
                        (0., 0., 1.)
                    )
                    v3 = self.geom_store.add_vertex(
                        (x, y_right, z_bottom),
                        (0., 0., 1.)
                    )
                    v4 = self.geom_store.add_vertex(
                        (x, y_right, z_top),
                        (0., 0., 1.)
                    )
                    if ccw:
                        self.geom_store.add_triangle(v1, v2, v3)
                        self.geom_store.add_triangle(v3, v4, v1)
                    else:
                        self.geom_store.add_triangle(v1, v4, v3)
                        self.geom_store.add_triangle(v3, v2, v1)
            ccw = False
