"""
Modular cuboid shape created procedurally inside the extents of a bounding box.
"""
from typing import List
from typing import Optional
from typing import Tuple

from panda3d.core import Vec3

from procshape.shape import Shape
from procshape.shape.plane import Plane


class Cuboid(Shape):
    """
    A simple cuboid shape to use as basis for subclassing shapes based on
    cuboids.

    Arguments:
        bounding_box: Vec3 half-extent of the shape
        origin_offset: Vec3, specifies the offset of the origin
    """
    def __init__(self, bounding_box, origin_offset=Vec3(0)):
        # type: (Vec3, Optional[Vec3]) -> None
        """Minimal setup requirements for a cuboid shape."""
        super(Cuboid, self).__init__('CuboidShape')
        self.__bounding_box__ = bounding_box
        self.__origin_offset__ = origin_offset
        self.__generate_initial__()

    @property
    def bounding_box(self):
        return self.__bounding_box__

    @property
    def origin_offset(self):
        return self.__origin_offset__

    def __generate_initial__(self):
        for n in [Vec3(1, 0, 0), Vec3(0, 1, 0), Vec3(0, 0, 1)]:
            bounds = []  # type: List[float, ...]
            for i, v in enumerate(n):
                if not v:
                    bounds.append(self.bounding_box[i])
            bounds = tuple(bounds)  # type: Tuple[float, float]
            origin = n * 1
            origin.componentwise_mult(self.bounding_box)
            p = Plane(
                tuple(origin + self.origin_offset),
                bounds,
                tuple(n)
            )
            self.geom_store.extend(p.geom_store)
            p = Plane(
                tuple(-origin + self.origin_offset),
                bounds,
                tuple(-n)
            )
            self.geom_store.extend(p.geom_store)
