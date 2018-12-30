"""
Some helper functions for vector math
"""
from typing import Optional

import numpy as np
from numba import jit

from procshape.helpers.types import NPA


def triangle_face_normal(p1, p2, p3, normalized=False):
    # type: (NPA, NPA, NPA, Optional[bool]) -> NPA
    """
    Returns orthogonal face normal for point arrays with the shape (..., 3)

    Arguments:
        p1: numpy.ndarray of shape (..., 3) as vertex A
        p2: numpy.ndarray of shape (..., 3) as vertex B
        p3: numpy.ndarray of shape (..., 3) as vertex C
        normalized: Optional bool if the normal(s) should be normalized to unit
            length.
    """
    if False in (isinstance(p1, NPA), isinstance(p2, NPA), isinstance(p3, NPA)):
        raise ValueError(f'Expected type numpy.ndarray for args: p1/p2/p3, got '
                         f'{type(p1)}/{type(p2)}/{type(p3)} instead!')
    if not (p1.shape == p2.shape == p3.shape):
        raise ValueError(f'Expected same shape for args: p1/p2/p3, got '
                         f'{p1.shape}/{p2.shape}/{p3.shape} instead!')
    u = p2 - p1
    v = p3 - p1
    normal = np.empty(p1.shape[:-1] + (3, ), dtype=np.float32)
    normal[..., 0] = u[..., 1] * v[..., 2] - u[..., 2] * v[..., 1]
    normal[..., 1] = u[..., 2] * v[..., 0] - u[..., 0] * v[..., 2]
    normal[..., 2] = u[..., 0] * v[..., 1] - u[..., 1] * v[..., 0]
    if normalized:
        return normalize(normal)
    return normal


@jit(nopython=True)  # Todo: test best speed with jit (off, nopython, parallel)
def normalize(vertices):
    # type: (NPA) -> NPA
    """
    Returns a normalized numpy.ndarray of the same shape as ``vertices``.

    Arguments:
        vertices: numpy.ndarray of shape (..., 3)
    """
    if not isinstance(vertices, NPA):
        raise ValueError(f'Expected numpy.ndarray for vertices, got '
                         f'{type(vertices)} instead!')
    if vertices.shape[-1] != 3:
        raise ValueError(f'Expected numpy.ndarray of shape (..., 3), got '
                         f'{vertices.shape} instead!')
    lengths = np.sqrt(
        (vertices ** 2).sum(axis=-1).reshape(*vertices.shape[:-1], 1)
    )
    return vertices / lengths


def cross(va, vb):
    # type: (NPA, NPA) -> NPA
    return np.array([
        va[1] * vb[2] - vb[1] * va[2],
        va[2] * vb[0] - vb[2] * va[0],
        va[0] * vb[1] - vb[0] * va[1],
    ], dtype=np.float32)


@jit(nopython=True)  # Todo: test best speed with jit (off, nopython, parallel)
def dot(va, vb):
    # type: (NPA, NPA) -> NPA
    """
    Returns the dot product for arrays of shape (..., n). Does not normalize
    the vectors beforehand.

    Arguments:
        va: numpy.ndarray of shape (..., n) containing vectors of Rn
        vb: numpy.ndarray of shape (..., n) containing vectors of Rn
    """
    if not isinstance(va, NPA) or not isinstance(vb, NPA):
        raise ValueError(f'Expected type numpy.ndarray, got va={type(va)}, '
                         f'vb={type(vb)} instead!')
    if va.shape != vb.shape:
        raise ValueError('Arguments va and vb must be of the same shape!')
    return sum([va[..., i] * vb[..., i] for i in range(va.shape[-1])])


@jit(nopython=True)  # Todo: test best speed with jit (off, nopython, parallel)
def point_on_line_dist(lpa, lpb, p):
    # type: (NPA, NPA, NPA) -> float
    """
    Returns the absolute difference between the distance from point ``lpa``
    to point ``lpb`` and the sum of the distances of segments ``p`` - ``lpa``
    and ``lpb`` - ``p``.
    Can be used for checks if a point in 3D space lies or almost lies on the
    line between ``lpa`` and ``lpb``

    Arguments:
         lpa: numpy.ndarray of shape (2,) or (3,) as point A in the line
         lpb: numpy.ndarray of shape (2,) or (3,) as point B in the line
         p: numpy.ndarray of shape (2,) or (3,) as point to check
    """
    line_dist = np.sqrt(((lpb - lpa) ** 2).sum())
    seg_a_dist = np.sqrt(((p - lpa) ** 2).sum())
    seg_b_dist = np.sqrt(((lpb - p) ** 2).sum())
    return abs(line_dist - seg_a_dist - seg_b_dist)
