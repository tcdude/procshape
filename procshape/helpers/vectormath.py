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
    return _normalize(vertices)


@jit(nopython=True)  # Todo: test best speed with jit (off, nopython, parallel)
def _normalize(vertices):
    lengths = np.sqrt(
        (vertices ** 2).sum(axis=-1).reshape(*vertices.shape[:-1], 1)
    )
    return vertices / lengths


@jit(nopython=True)  # Todo: test best speed with jit (off, nopython, parallel)
def cross(va, vb):
    # type: (NPA, NPA) -> NPA
    out = np.empty(va.shape, dtype=np.float32)
    out[..., 0] = va[..., 1] * vb[..., 2] - vb[..., 1] * va[..., 2]
    out[..., 1] = va[..., 2] * vb[..., 0] - vb[..., 2] * va[..., 0]
    out[..., 2] = va[..., 0] * vb[..., 1] - vb[..., 0] * va[..., 1]
    return out


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
    return _dot(va, vb)


@jit(nopython=True)  # Todo: test best speed with jit (off, nopython, parallel)
def _dot(va, vb):
    return (va * vb).sum(axis=-1)
