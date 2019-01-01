from unittest import TestCase
import numpy as np

from procshape.helpers import shapetools

# Test Data
vertices = np.array(
    [
        [-1, 0, 0],
        [1, 0, 0],
        [1, 1, 0],
        [-1, 1, 0],
        [1, 0, 0],
        [-1, 0, 0],
        [-1, 0, -1],
        [1, 0, -1],
    ],
    dtype=np.float32
)
colors = np.array(
    [
        [1, 0, 0, 1],
        [0, 1, 0, 1],
        [1, 1, 0, 1],
        [0, 1, 1, 1],
        [0, 0, 1, 1],
        [0, 1, 1, 1],
        [1, 0, 1, 1],
        [1, 1, 1, 1],
    ],
    dtype=np.float32
)
triangles = np.array(
    [
        [0, 1, 2],
        [2, 3, 0],
        [4, 5, 6],
        [6, 7, 4],
    ],
    dtype=np.int64
)
target_distance = 2.0
subdivisions = 1
expected_vertices = np.array(
    [
        [-1., 0., -1.],
        [-1., 0., 0.],
        [-1., 0., 0.],
        [-1., 1., 0.],
        [0., 0., -0.5],
        [0., 0.5, 0.],
        [1., 0., -1.],
        [1., 0., 0.],
        [1., 0., 0.],
        [1., 1., 0.]
    ],
    dtype=np.float32
)
expected_colors = np.array(
    [
        [1., 0., 1., 1.],
        [0., 1., 1., 1.],
        [1., 0., 0., 1.],
        [0., 1., 1., 1.],
        [0.5, 0., 1., 1.],
        [1., 0.5, 0., 1.],
        [1., 1., 1., 1.],
        [0., 0., 1., 1.],
        [0., 1., 0., 1.],
        [1., 1., 0., 1.]
    ],
    dtype=np.float32
)
expected_triangles = np.array(
    [
        [5, 8, 9],
        [5, 3, 2],
        [4, 1, 0],
        [4, 6, 7],
        [5, 2, 8],
        [5, 9, 3],
        [4, 7, 1],
        [4, 0, 6]
    ],
    dtype=np.int64
)


class TestShapetoolsMethods(TestCase):
    def test_collapse_min_cost_edge(self):
        self.fail()

    def test_compute_edge_collapse_cost(self):
        self.fail()

    def test_get_edges(self):
        _, test_tri, _ = shapetools.subdivide_triangles(
            vertices,
            triangles,
            colors,
            subdivisions
        )
        res_edges = shapetools.get_edges(expected_triangles)
        self.assertEqual(4, len(res_edges))

    def test_get_vertex_triangles(self):
        for i in range(len(vertices)):
            result_set = shapetools.get_vertex_triangles(
                triangles,
                i
            )
            self.assertTrue(np.isin(triangles[result_set], i).any(axis=1).all())

    def test_get_vertex_id(self):
        for i in range(len(vertices)):
            result_id = shapetools.get_vertex_id(
                vertices[i],
                colors[i],
                vertices,
                colors
            )
            self.assertTrue(i == result_id)

    def test_subdivide_triangles(self):
        res_vert, res_tri, res_col = shapetools.subdivide_triangles(
            vertices,
            triangles,
            colors,
            subdivisions
        )
        self.check_subdivide(res_col, res_tri, res_vert)

    def test_subdivide_triangles_dist(self):
        res_vert, res_tri, res_col = shapetools.subdivide_triangles_dist(
            vertices,
            triangles,
            colors,
            target_distance
        )
        self.check_subdivide(res_col, res_tri, res_vert)

    def check_subdivide(self, res_col, res_tri, res_vert):
        self.assertEqual(len(res_vert), len(expected_vertices))
        self.assertEqual(len(res_tri), len(expected_triangles))
        self.assertEqual(len(res_col), len(expected_colors))
        self.assertTrue(
            (
                    np.sort(res_vert, axis=0) ==
                    np.sort(expected_vertices, axis=0)
            ).all(),
            f'Vertices are different:\n expected {expected_vertices}\n got '
            f'{res_vert}'
        )
        self.assertTrue(
            (
                    np.sort(res_col, axis=0) ==
                    np.sort(expected_colors, axis=0)
            ).all(),
            f'Colors are different:\n expected {expected_colors}\n got '
            f'{res_col}'
        )
        all_triangles = True
        for tri in res_tri:
            if np.isin(expected_triangles, tri).all(axis=1).any():
                continue
            elif np.isin(
                    expected_triangles,
                    np.roll(tri, -1)
            ).all(axis=1).any():
                continue
            elif np.isin(
                    expected_triangles,
                    np.roll(tri, -2)
            ).all(axis=1).any():
                continue
            else:
                all_triangles = False
                break
        self.assertTrue(
            all_triangles,
            f'Triangles are different:\n expected {expected_triangles}\n got '
            f'{res_tri}'
        )
