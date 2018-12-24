"""
Some helper functions for vector math
"""

import numpy as np

from procshape.helpers.types import NPA


def cross(va, vb):
    # type: (NPA, NPA) -> NPA
    return np.array([
        va[1] * vb[2] - vb[1] * va[2],
        va[2] * vb[0] - vb[2] * va[0],
        va[0] * vb[1] - vb[0] * va[1],
    ], dtype=np.float32)
