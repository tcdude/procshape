"""
The StaticObjectProducer class is a blank shape, meant to be subclassed and used
 to procedurally generate shapes of varying complexity to use in 3D
 applications.
The basic implementation simply generates the geom of a cube and demonstrates
 a simple LOD mechanism which simply subdivides the cube into more triangles.
 You will need to override the generate(lod=None) method in your subclass
"""

from procshape.producer.base import BaseProducer


class StaticObjectProducer(BaseProducer):
    def generate(self, lod=None):
        pass
