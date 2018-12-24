"""
Configurable consumer/producer, that provides geometry for use in Panda3D.
Provides the GeomStreamer parent class for you to subclass and configure
 with your specific needs.

Consumes:
Localized geometry from one or more GeometryProducer object(s).
TerrainProducer and PlanetProducer objects, will generate Geometry
 and optionally use quadtree LOD.
ObjectProducer will provide geometry for a user defined number of
 LOD levels

Produces:
Panda3D Node objects (GeomNode, Bullet(Rigid)BodyNode), including
 TransformState objects, for you to use in your application.
"""


class GeomStreamer(object):
    pass
