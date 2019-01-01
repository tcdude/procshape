"""
Helper methods for the procshape.shape module
"""
from typing import List
from typing import Tuple

import numpy as np
from numba import jit

from procshape.helpers.types import NPA
from procshape.helpers.types import V3
from procshape.helpers.types import V4
from procshape.helpers import vectormath


@jit(nopython=True)  # Todo: test best speed with jit (off, nopython, parallel)
def collapse_min_cost_edge(vertices, triangles, face_normals, edges, costs):
    # type: (NPA, NPA, NPA, NPA, NPA) -> Tuple[NPA, NPA, NPA, NPA, List]
    """
    Returns a tuple: (``triangles``, ``face_normals``, ``edges``, ``costs``,
    ``history``), where ``triangles``, ``face_normals``, ``edges`` and ``costs``
    are the updated arrays after the collapse and history is a list of actions
    to reverse the collapse (e.g. [(triangle_old, triangle_new), ...] where
    triangle_old and triangle_new stand for the triangle before and after the
    collapse. triangle_new can be either an array of the triangle indices for
    modified triangles and None for removed triangles)

    Arguments:
        vertices: numpy.ndarray of vertices as indexed by ``triangles`` and
            ``edges``
        triangles: numpy.ndarray of triangle indices
        face_normals: numpy.ndarray of face normals indexed identically to
            ``triangles``
        edges: numpy.ndarray of all collapsible edges in the mesh
        costs: numpy.ndarray of the cost for an edge collapse, indexed
            identically to ``edges``
    """
    cost_min = costs.argmin()
    u = edges[cost_min, 0]
    v = edges[cost_min, 1]
    history = [
        (triangles[edges[cost_min, 2]].copy(), None),
        (triangles[edges[cost_min, 3]].copy(), None),
    ]

    # Remove and update triangles
    triangles = np.delete(triangles, edges[cost_min, 2:])
    neighbors = get_vertex_triangles(triangles, u)
    for neighbor in neighbors:
        old = triangles[neighbor].copy()
        triangles[neighbor, np.argwhere(triangles[neighbor] == u)[0, 0]] = v
        new = triangles[neighbor].copy()
        history.append((old, new))

    # Recalculate face normal
    new_normals = vectormath.triangle_face_normal(
        vertices[triangles[neighbors, 0]],
        vertices[triangles[neighbors, 1]],
        vertices[triangles[neighbors, 2]],
        True
    )
    face_normals[neighbors] = new_normals

    # Remove and update edges
    edges = np.delete(edges, cost_min)
    costs = np.delete(costs, cost_min)
    edges[np.argwhere(edges[..., 0] == u).T[0], 0] = v
    edges[np.argwhere(edges[..., 1] == u).T[0], 1] = v

    # Recompute edge collapse cost for v
    sub_edges, sub_costs = compute_edge_collapse_cost(
        vertices,
        triangles[np.argwhere(triangles == v).T[0]],
        face_normals
    )
    for i, edge in enumerate(sub_edges):
        edge_id = np.where(
            (edges[..., 0] == edge[0]) &
            (edges[..., 1] == edge[1])
        )[0][0]
        costs[edge_id] = sub_costs[i]
    return triangles, face_normals, edges, costs, history


@jit(nopython=True)  # Todo: test best speed with jit (off, nopython, parallel)
def compute_edge_collapse_cost(vertices, triangles, face_normals):
    # type: (NPA, NPA, NPA) -> Tuple[NPA, NPA]
    """Returns a tuple: (edges, costs) for each valid edge in the mesh"""
    edges = get_edges(triangles)
    edge_lengths = vertices[edges[..., 1]] - vertices[edges[..., 0]]
    edge_lengths = (edge_lengths ** 2).sum(axis=-1)
    costs = np.zeros(edge_lengths.shape, dtype=np.float32)
    for i, edge in enumerate(edges):
        neighbors = get_vertex_triangles(triangles, edge[0])
        curvature = 0.0
        mincurv = 1.0
        for neighbor in neighbors:
            dotprod_a = sum([
                face_normals[edges[..., 2], d] * face_normals[neighbor, d]
                for d in [0, 1, 2]
            ])
            dotprod_b = sum([
                face_normals[edges[..., 3], d] * face_normals[neighbor, d]
                for d in [0, 1, 2]
            ])
            mincurv = min((1.0 - dotprod_a) / 2.0, (1.0 - dotprod_b) / 2.0)
        curvature = max(curvature, mincurv)
        costs[i] = curvature * edge_lengths[i]
    return edges, costs


def get_edges(triangles):
    # type: (NPA) -> NPA
    """
    Returns all joining edges found in ``triangles`` in the form of:

    [[u, v, tri_id_from, tri_id_to], ...]

    No edges are returned for triangles, that have one or more non joining
    edges. All edges are mapped twice, once for each direction (e.g. u->v, v->u)
    """
    triangle_sides = triangles[..., [[0, 1], [1, 2], [2, 0]]]
    check_sides = np.flip(triangle_sides, -1)
    edges = []
    for tri_id, triangle in enumerate(triangle_sides):
        tmp_edges = []
        for side in triangle:
            print(side)
            res = np.where(
                ((check_sides[..., 0] == side[0]) &
                 (check_sides[..., 1] == side[1])) |
                ((check_sides[..., 0] == side[1]) &
                 (check_sides[..., 1] == side[0]))
            )
            if len(res[0]):
                tmp_edges.append(list(side) + [tri_id, res[0][0]])
                tmp_edges.append(list(np.flip(side)) + [res[0][0], tri_id])
        if len(tmp_edges) == 3:
            edges += tmp_edges
    if len(edges):
        edges = np.unique(edges, return_index=True, axis=0).astype(np.int64)
        return edges
    return np.empty((0, 2), dtype=np.int64)


@jit(nopython=True)  # Todo: test best speed with jit (off, nopython, parallel)
def get_vertex_triangles(triangles, vertex_id):
    return np.where(triangles == vertex_id)[0]


def get_vertex_id(point, color, vertices, colors):
    # type: (V3, V4, NPA, NPA) -> int
    """Returns matching array index or -1 if none was found"""
    for i in range(len(vertices)):
        res = True
        for k in range(4):
            if k < 3:
                if point[k] != vertices[i][k] or color[k] != colors[i][k]:
                    res = False
                    break
            else:
                if color[k] != colors[i][k]:
                    res = False
                    break
        if res:
            return i
    return -1


# @jit(parallel=True)
def clean_up_mesh(vertices, triangles, colors):
    combined = np.empty(vertices.shape[:-1] + (7, ), dtype=np.float32)
    combined[..., :3] = vertices
    combined[..., 3:] = colors
    _, indices, inverse = np.unique(
        combined,
        return_index=True,
        return_inverse=True,
        axis=0)
    vertices = vertices[indices]
    colors = colors[indices]
    triangles = inverse[triangles]
    return vertices, triangles, colors


def subdivide_triangles(vertices, triangles, colors, subdivisions=1):
    # type: (NPA, NPA, NPA, int) -> Tuple[NPA, NPA, NPA]
    """Subdivides every triangle on its longest side ``subdivisions`` times"""
    distance_vec = (vertices[np.roll(triangles, -1, 1)] - vertices[triangles])
    indices = (distance_vec ** 2).sum(axis=2).argmax(axis=1)
    assert (len(distance_vec) == len(indices))
    old_tri_len = len(triangles)
    new_triangles = np.empty((old_tri_len * 2, 3), dtype=np.int64)
    new_triangles[:old_tri_len] = triangles.copy()
    old_vert_len = len(vertices)
    new_vertices = np.empty(
        (old_vert_len + old_tri_len, 3),
        dtype=np.float32
    )
    new_vertices[:old_vert_len] = vertices.copy()
    new_colors = np.empty(
        (old_vert_len + old_tri_len, 4),
        dtype=np.float32
    )
    new_colors[:old_vert_len] = colors.copy()
    for i, start_idx in enumerate(indices):
        new_tri = triangles[i].copy()
        dist = distance_vec[i, start_idx] * 0.5
        new_point = dist + vertices[triangles[i, start_idx]]
        new_point_id = old_vert_len + i
        new_vertices[new_point_id] = new_point
        to_idx = (start_idx + 1) % 3
        new_color = colors[triangles[i][start_idx]].copy()
        new_color += colors[triangles[i][to_idx]].copy()
        new_color *= 0.5
        new_colors[new_point_id] = new_color
        new_triangles[i][to_idx] = new_point_id
        new_tri[start_idx] = new_point_id
        new_triangles[i + old_tri_len] = new_tri
    new_vertices, new_triangles, new_colors = clean_up_mesh(
        new_vertices,
        new_triangles,
        new_colors)
    if subdivisions > 1:
        return subdivide_triangles(
            new_vertices,
            new_triangles,
            new_colors,
            subdivisions - 1
        )
    else:
        return new_vertices, new_triangles, new_colors


def subdivide_triangles_dist(vertices, triangles, colors, target_distance=2.0):
    # type: (NPA, NPA, NPA, float) -> Tuple[NPA, NPA, NPA]
    _check_dist = target_distance ** 2
    num_new_rows = 1
    while num_new_rows:
        dist_vec = (vertices[np.roll(triangles, -1, 1)] - vertices[triangles])
        len_vec = (dist_vec ** 2).sum(axis=2)
        vec_max = len_vec.max(axis=1)
        rows = np.where(vec_max > _check_dist)[0]
        num_new_rows = len(rows)
        if num_new_rows == 0:
            break
        indices = len_vec[rows].argmax(axis=1)
        colors, triangles, vertices = _subdivide(vertices, triangles, colors,
                                                 indices, rows, dist_vec,
                                                 num_new_rows)
        vertices, triangles, colors = clean_up_mesh(
            vertices,
            triangles,
            colors)
    return vertices, triangles, colors


@jit(nopython=True)
def _subdivide(vertices, triangles, colors, indices, rows, distance_vec,
               num_new_rows):
    # type: (NPA, NPA, NPA, NPA, NPA, NPA, int) -> Tuple[NPA, NPA, NPA]
    old_tri_len = len(triangles)
    new_triangles = np.empty(
        (old_tri_len + num_new_rows, 3),
        dtype=np.int64
    )
    new_triangles[:old_tri_len] = triangles.copy()
    old_vert_len = len(vertices)
    new_vertices = np.empty(
        (old_vert_len + num_new_rows, 3),
        dtype=np.float32
    )
    new_vertices[:old_vert_len] = vertices.copy()
    new_colors = np.empty(
        (old_vert_len + num_new_rows, 4),
        dtype=np.float32
    )
    new_colors[:old_vert_len] = colors.copy()
    for i, start_idx in enumerate(indices):
        new_tri = triangles[rows[i]].copy()
        dist = distance_vec[rows[i], start_idx] * 0.5
        new_point = vertices[triangles[rows[i], start_idx]] + dist
        new_point_id = old_vert_len + i
        new_vertices[new_point_id] = new_point
        to_idx = (start_idx + 1) % 3
        new_color = colors[triangles[rows[i]][start_idx]].copy()
        new_color += colors[triangles[rows[i]][to_idx]].copy()
        new_color *= 0.5
        new_colors[new_point_id] = new_color
        new_triangles[rows[i]][to_idx] = new_point_id
        new_tri[start_idx] = new_point_id
        new_triangles[i + old_tri_len] = new_tri
    vertices = new_vertices
    triangles = new_triangles
    colors = new_colors
    return colors, triangles, vertices
