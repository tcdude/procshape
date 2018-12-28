"""
Various helper modules, used across the package
"""
import numpy as np


def get_triangle_strip(top_len, bottom_len):
    """
    Generates Triangles indices from two lines of points of various length.

    :param top_len: The length of the upper line (serves to assure ccw winding)
    :param bottom_len: The length of the lower line
    :return: A list containing two lists for each entry, representing the top
             and bottom indices of a triangle.
    """
    if top_len < bottom_len:
        reverse_long_row = True
    else:
        reverse_long_row = False
    min_len = min(top_len, bottom_len)
    max_len = max(top_len, bottom_len)
    short_indices = np.linspace(
        0.999999,
        min_len - 0.999999,
        max_len,
        dtype=np.int64
    )
    long_indices = np.arange(max_len)
    long_row = []
    short_row = []
    for i in range(max_len):
        n = (i + 1) % max_len
        short_pos = i
        if short_indices[i] != short_indices[n]:
            long_row.append([long_indices[i]])
            short_pos = n
            if reverse_long_row:
                short_row.append([short_indices[n], short_indices[i]])
            else:
                short_row.append([short_indices[i], short_indices[n]])
        short_row.append([short_indices[short_pos]])
        long_row.append([long_indices[i], long_indices[n]]) \
            if reverse_long_row else long_row.append([long_indices[n],
                                                      long_indices[i]])
    if top_len == max_len:
        return [(top, short_row[i]) for i, top in enumerate(long_row)]
    return [(top, long_row[i]) for i, top in enumerate(short_row)]
