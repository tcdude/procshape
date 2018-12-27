"""
Modular cylinder shapes created procedurally. Can follow a path and vary in
 radius.
"""
from panda3d.core import NodePath
from panda3d.core import Vec3

from procshape.helpers import get_triangle_strip
from procshape.helpers.types import V3
from procshape.shape import Shape


class SingleCylinder(Shape):
    def __init__(
            self,
            circle_points,
            start_point,
            start_radius,
            end_point,
            end_radius
    ):
        # type: (int, V3, float, V3, float) -> None
        super(SingleCylinder, self).__init__('CylinderShape')
        self.__circle_points__ = circle_points
        self.__start_point__ = Vec3(start_point)
        self.__start_radius__ = start_radius
        self.__end_point__ = Vec3(end_point)
        self.__end_radius__ = end_radius
        self.__generate_initial__()

    # noinspection PyArgumentList
    def __generate_initial__(self):
        w = NodePath('world')
        c = NodePath('center')
        o = NodePath('orientation')
        t = NodePath('target')
        d = NodePath('draw')
        c.reparent_to(w)
        t.reparent_to(w)
        o.reparent_to(c)
        d.reparent_to(o)
        c.set_pos(self.__start_point__)
        t.set_pos(self.__end_point__)
        c.look_at(t)
        h_step = 360.0 / self.__circle_points__
        start_circle = []
        end_circle = []
        for h in range(self.__circle_points__):
            o.set_r(h * h_step)
            c.set_pos(self.__start_point__)
            d.set_pos(self.__start_radius__, 0, 0)
            start_circle.append(d.get_pos(w))
            c.set_pos(self.__end_point__)
            d.set_pos(self.__end_radius__, 0, 0)
            end_circle.append(d.get_pos(w))
        color = (1.0,) * 4
        vertex_ids = [
            [self.geom_store.add_vertex(tuple(self.__start_point__), color)],
            [self.geom_store.add_vertex(tuple(p), color) for p in start_circle],
            [self.geom_store.add_vertex(tuple(p), color) for p in end_circle],
            [self.geom_store.add_vertex(tuple(self.__end_point__), color)]
        ]
        for i in range(3):
            for upper, lower in get_triangle_strip(
                    len(vertex_ids[i + 1]),
                    len(vertex_ids[i])
            ):
                vertices = [vertex_ids[i][v] for v in lower]
                vertices += [vertex_ids[i + 1][v] for v in upper]
                self.geom_store.add_triangle(*vertices)
