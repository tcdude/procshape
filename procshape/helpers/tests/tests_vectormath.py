from unittest import TestCase

import numpy as np

from procshape.helpers import vectormath

# Test Data
vertices = np.array(
    [
        [-1, 0, 0],
        [1, 0, 0],
        [1, 1, 0],
        [-1, 1, 0],
    ],
    dtype=np.float32
)
triangles = np.array(
    [
        [0, 1, 2],
        [2, 3, 0],
        [2, 1, 0],
        [0, 3, 2],
    ],
    dtype=np.int64
)
expected_face_normals = np.array(
    [
        [0, 0, 1],
        [0, 0, 1],
        [0, 0, -1],
        [0, 0, -1],
    ],
    dtype=np.float32
)
expected_vertex_normals = np.array(
    [
        [-1, 0, 0],
        [1, 0, 0],
        [0.7071067690849304, 0.7071067690849304, 0.0],
        [-0.7071067690849304, 0.7071067690849304, 0.0],
    ],
    dtype=np.float32
)
expected_cross = np.array(
    [
        [0.0, 0.0, -0.0],
        [0.0, 0.0, 1.0],
        [0.0, -0.0, 2.0],
        [0.0, 0.0, 1.0]],
    dtype=np.float32
)
expected_dot = np.array(
    [-1.0, 1.0, 0.0, 1.0],
    dtype=np.float32
)


class TestVectormathMethods(TestCase):
    def test_triangle_face_normal(self):
        result = vectormath.triangle_face_normal(
            vertices[triangles[..., 0]],
            vertices[triangles[..., 1]],
            vertices[triangles[..., 2]],
            True
        )
        self.assertTrue(np.isclose(result, expected_face_normals).all())

    def test_normalize(self):
        result = vectormath.normalize(vertices)
        self.assertTrue(np.isclose(result, expected_vertex_normals).all())

    def test_cross(self):
        result = vectormath.cross(vertices, np.roll(vertices, -1, 0))
        self.assertTrue(np.isclose(result, expected_cross).all())

    def test_dot(self):
        result = vectormath.dot(vertices, np.roll(vertices, -1, 0))
        self.assertTrue(np.isclose(result, expected_dot).all())
