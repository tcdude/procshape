"""
Modular spheroid shape created form a spherized cuboid.
"""
from typing import Optional

from panda3d.core import Vec3

from procshape.shape import Shape
from procshape.shape.cuboid import Cuboid


class Spheroid(Shape):
    """
    A simple spheroid shape to use as basis for subclassing shapes based on
    spheroids.

    Arguments:
        bounding_box: Vec3 half-extent of the shape
        origin_offset: Vec3, specifies the offset of the origin
    """
    def __init__(
            self,
            bounding_box,
            origin_offset=Vec3(0),
            subdiv=None,
            subdiv_dist=None
    ):
        # type: (Vec3, Optional[Vec3], Optional[int], Optional[float]) -> None
        super(Spheroid, self).__init__('SpheroidShape')
        if None not in (subdiv, subdiv_dist):
            raise ValueError('Specify either subdiv or subdiv_dist, '
                             'not both!')
        self.__bounding_box__ = bounding_box
        self.__origin_offset__ = origin_offset
        self.__subdiv_type__ = 'fixed' if subdiv is not None else 'dist'
        self.__subdiv_arg__ = subdiv or subdiv_dist \
            if (subdiv, subdiv_dist) != (None, None) else 2.0
        self.__generate_initial__()

    @property
    def bounding_box(self):
        return self.__bounding_box__

    @property
    def origin_offset(self):
        return self.__origin_offset__

    def geom_node_trigger(self):
        self.__update_spheroid__()

    def __update_spheroid__(self):
        self.geom_store.to_unit_sphere()
        _ = self.geom_store * self.bounding_box

    def __generate_initial__(self):
        c = Cuboid(self.bounding_box)
        c.subdivide(2)
        c.geom_store.to_unit_sphere()
        _ = c.geom_store * self.bounding_box
        if self.__subdiv_type__ == 'fixed' and self.__subdiv_arg__ > 2:
            c.subdivide(self.__subdiv_arg__ - 2)
        else:
            c.subdivide_dist(self.__subdiv_arg__)
        self.geom_store.extend(c.geom_store)
        # self.__update_spheroid__()
