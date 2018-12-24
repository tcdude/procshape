"""
Simple Plane shape for use in Terrains, Water and Foliage.
Can be two-sided where useful (Water from below, Foliage, etc.).
While adding vertices it is possible to specify 2 colors, one for each
side (only has effect if a two-sided Plane is being used).
"""

from typing import Optional

import numpy as np

from procshape.helpers.types import V2
from procshape.helpers.types import V3
from procshape.shape import Shape


class Plane(Shape):
    """
    Generates a plane

    Arguments:
        origin: center point of the plane
        bounds: positive half-width (u) and -length (v) of the plane
        normal: the normal vector of the plane
        two_sided: if True makes the plane two-sided
    """
    def __init__(self, origin, bounds, normal, two_sided=False):
        # type: (V3, V2, V3, Optional[bool]) -> None
        super(Plane, self).__init__('PlaneShape')
        self.__origin__ = np.array(origin, dtype=np.float32)
        self.__bounds__ = np.array(bounds, dtype=np.float32)
        self.__normal__ = np.array(normal, dtype=np.float32)
        self.__two_sided__ = two_sided
        self.__generate_initial__()

    def __generate_initial__(self):
        dim_max = np.abs(self.__normal__).argmax()
        if dim_max == 0:  # x
            u = np.array([0, 1, 0], dtype=np.float32)
        elif dim_max == 1:  # y
            u = np.array([-1, 0, 0], dtype=np.float32)
        else:  # z
            u = np.array([1, 0, 0], dtype=np.float32)
        if self.__normal__[dim_max] < 0:
            u *= -1
        v_tangent = np.cross(self.__normal__, u)
        u_tangent = np.cross(self.__normal__, -v_tangent)
        u_tangent *= self.__bounds__[0]
        v_tangent *= self.__bounds__[1]
        p1 = self.__origin__ - u_tangent - v_tangent
        p2 = self.__origin__ + u_tangent - v_tangent
        p3 = self.__origin__ + u_tangent + v_tangent
        p4 = self.__origin__ - u_tangent + v_tangent
        p1 = self.__geom_store__.add_vertex(tuple(p1), color=(1., 1., 1., 1.))
        p2 = self.__geom_store__.add_vertex(tuple(p2), color=(1., 1., 1., 1.))
        p3 = self.__geom_store__.add_vertex(tuple(p3), color=(1., 1., 1., 1.))
        p4 = self.__geom_store__.add_vertex(tuple(p4), color=(1., 1., 1., 1.))
        self.__geom_store__.add_triangle(p1, p2, p3)
        self.__geom_store__.add_triangle(p3, p4, p1)
        if self.__two_sided__:
            self.__geom_store__.add_triangle(p1, p3, p2)
            self.__geom_store__.add_triangle(p3, p1, p4)
