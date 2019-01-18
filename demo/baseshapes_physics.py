"""
Example that shows loading of simple shapes
"""
import sys
import random

from direct.gui.OnscreenText import OnscreenText
from direct.showbase.ShowBase import ShowBase
# noinspection PyProtectedMember
from direct.showbase.ShowBaseGlobal import globalClock
from panda3d.bullet import BulletPlaneShape
from panda3d.bullet import BulletRigidBodyNode
from panda3d.bullet import BulletWorld
from panda3d.core import TextNode
from panda3d.core import Vec3

from procshape.helpers.bullet import get_bullet_shape
from procshape.shape.cuboid import Cuboid
from procshape.shape.cylinder import SingleCylinder
from procshape.shape.plane import Plane
from procshape.shape.spheroid import Spheroid
from procshape.shape.geomstore import Shape


class Demo(ShowBase):
    def __init__(self):
        ShowBase.__init__(self)
        # Bullet Physics Setup
        self.world = BulletWorld()
        self.world.setGravity(Vec3(0, 0, -9.81))

        shape = BulletPlaneShape(Vec3(0, 0, 1), 1)
        node = BulletRigidBodyNode('Ground')
        node.addShape(shape)
        ground_np = self.render.attachNewNode(node)
        ground_np.setPos(0, 0, -60)
        # noinspection PyArgumentList
        self.world.attach(node)

        # Mouse and Camera Setup
        self.disable_mouse()
        self.camera.set_pos(20, -150, 5)
        self.follow_np = self.render.attach_new_node('Follow')

        # Instructions
        self.add_instruction('(p) to generate a Plane', 0.06)
        self.add_instruction('(b) to generate a Cuboid/Box', 0.12)
        self.add_instruction('(s) to generate a Spheroid', 0.18)
        self.add_instruction('(c) to generate a Cylinder', 0.24)
        self.add_instruction('(f1) to toggle wireframe', 0.30)
        self.add_instruction('(esc) to exit', 0.36)
        # Input
        self.accept('escape', sys.exit, [0])
        self.accept('p', self.generate_plane)
        self.accept('b', self.generate_cuboid)
        self.accept('s', self.generate_spheroid)
        self.accept('c', self.generate_cylinder)
        self.accept('f1', self.toggle_wireframe)

        # Shape
        self.shape = Shape()

        self.task_mgr.add(self.update, 'update')

    def add_instruction(self, text, y_pos, x_pos=0.06):
        _ = OnscreenText(
            text,
            pos=(x_pos, y_pos),
            fg=(1, 1, 1, 1),
            parent=self.a2dBottomLeft,
            scale=0.06,
            align=TextNode.ALeft
        )

    def update(self, task):
        dt = globalClock.getDt()
        self.world.doPhysics(dt)
        self.camera.look_at(self.follow_np)
        return task.cont

    # noinspection PyArgumentList
    def generate_plane(self):
        bounds = Vec3(*[random.uniform(5.0, 30.0) for _ in range(2)])
        pos = Vec3(
            *[random.uniform(-100.0, 100.0) for _ in range(2)],
            random.uniform(50.0, 400.0)
        )
        normal = Vec3(
            *[random.uniform(-1.0, 1.0) for _ in range(3)],
        )
        color = tuple(normal * 0.5 + 0.5) + (1.,)
        p = Plane((0., 0., 0.), bounds, normal, two_sided=True)
        p.subdivide_dist(2.0)
        p.geom_store.set_color(color)
        geom_node = p.geom_node
        plane_np = self.render.attach_new_node(geom_node)
        shape = get_bullet_shape(geom_node)
        node = BulletRigidBodyNode('Plane')
        node.set_mass(random.uniform(0.1, 1000.0))
        node.add_shape(shape)
        visual_np = self.render.attach_new_node(node)
        self.world.attach(node)
        plane_np.reparent_to(visual_np)
        self.follow_np.reparent_to(visual_np)
        visual_np.set_pos(pos)

    # noinspection PyArgumentList
    def generate_cuboid(self):
        bounding_box = Vec3(*[random.uniform(5.0, 30.0) for _ in range(3)])
        pos = Vec3(
            *[random.uniform(-100.0, 100.0) for _ in range(2)],
            random.uniform(50.0, 400.0)
        )
        hpr = Vec3(*[random.random() * 360 for _ in range(3)])
        c = Cuboid(bounding_box)
        c.subdivide_dist(2.0)
        c.geom_store.normals_as_color()
        geom_node = c.geom_node
        cuboid_np = self.render.attach_new_node(geom_node)
        shape = get_bullet_shape(geom_node)
        node = BulletRigidBodyNode('Cuboid')
        node.set_mass(random.uniform(0.1, 1000.0))
        node.add_shape(shape)
        visual_np = self.render.attach_new_node(node)
        self.world.attach(node)
        cuboid_np.reparent_to(visual_np)
        self.follow_np.reparent_to(visual_np)
        visual_np.set_pos(pos)
        visual_np.set_hpr(hpr)

    # noinspection PyArgumentList
    def generate_spheroid(self):
        bounding_box = Vec3(*[random.uniform(5.0, 30.0) for _ in range(3)])
        pos = Vec3(
            *[random.uniform(-100.0, 100.0) for _ in range(2)],
            random.uniform(50.0, 400.0)
        )
        hpr = Vec3(*[random.random() * 360 for _ in range(3)])
        s = self.shape.get_spheroid(bounding_box, Vec3(0))
        s.subdivide_triangles(8)
        s.to_unit_sphere()
        _ = s * bounding_box
        s.normals_as_color()
        geom_node = s.get_p3d_geom_node('Spheroid')
        # s = Spheroid(bounding_box, subdiv_dist=2.0)
        # s.geom_store.normals_as_color()
        # geom_node = s.geom_node
        spheroid_np = self.render.attach_new_node(geom_node)
        shape = get_bullet_shape(geom_node)
        node = BulletRigidBodyNode('Cuboid')
        node.set_mass(random.uniform(0.1, 1000.0))
        node.add_shape(shape)
        visual_np = self.render.attach_new_node(node)
        self.world.attach(node)
        spheroid_np.reparent_to(visual_np)
        self.follow_np.reparent_to(visual_np)
        visual_np.set_pos(pos)
        visual_np.set_hpr(hpr)

    # noinspection PyArgumentList
    def generate_cylinder(self):
        start_point = Vec3(*[random.uniform(-30.0, 30.0) for _ in range(3)])
        end_point = -start_point
        radii = [random.randint(0, 10) for _ in range(2)]
        pos = Vec3(
            *[random.uniform(-100.0, 100.0) for _ in range(2)],
            random.uniform(50.0, 400.0)
        )
        c = SingleCylinder(12, start_point, radii[0], end_point, radii[1])
        c.subdivide_dist(2.0)
        c.geom_store.normals_as_color()
        geom_node = c.geom_node
        cylinder_np = self.render.attach_new_node(geom_node)
        shape = get_bullet_shape(geom_node)
        node = BulletRigidBodyNode('Cuboid')
        node.set_mass(random.uniform(0.1, 1000.0))
        node.add_shape(shape)
        visual_np = self.render.attach_new_node(node)
        self.world.attach(node)
        cylinder_np.reparent_to(visual_np)
        self.follow_np.reparent_to(visual_np)
        visual_np.set_pos(pos)


if __name__ == '__main__':
    Demo().run()
