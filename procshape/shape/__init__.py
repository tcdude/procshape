"""
Provides basic shapes for PG. All shapes should be subclassed from Shape.
"""
from procshape.shape.geomstore import GeomStore


class Shape(object):
    def __init__(self, name='Shape'):
        self.__geom_store__ = GeomStore()
        self.name = name

    @property
    def geom_store(self):
        return self.__geom_store__

    @property
    def geom_node(self):
        self.geom_node_trigger()
        return self.geom_store.get_p3d_geom_node(self.name)

    def geom_node_trigger(self):
        """Method to override, to perform actions prior to export"""
        pass

    def subdivide(self, subdivisions=2):
        self.geom_store.subdivide_triangles(subdivisions)

    def subdivide_dist(self, target_distance=2):
        self.geom_store.subdivide_triangles_distance(target_distance)
