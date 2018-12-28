"""
Helper methods for bullet physics related operations
"""
from panda3d.bullet import BulletTriangleMesh
from panda3d.bullet import BulletTriangleMeshShape


def get_bullet_shape(geom_node):
    shape = BulletTriangleMesh()
    for i in range(geom_node.getNumGeoms()):
        geom = geom_node.getGeom(i)
        # state = geom_node.getGeomState(i)
        shape.addGeom(geom)
    shape = BulletTriangleMeshShape(shape, True)
    return shape
