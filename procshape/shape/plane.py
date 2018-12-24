"""
Simple Plane shape for use in Terrains, Water and Foliage.
Can be two-sided where useful (Water from below, Foliage, etc.).
"""
from procshape.shape.geomstore import GeomStore


class Plane(object):
    def __init__(self):
        self.__geom_store__ = GeomStore()
