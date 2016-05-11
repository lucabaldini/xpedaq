#!/usr/bin/env python
# Copyright (C) 2007--2016 the X-ray Polarimetry Explorer (XPE) team.
#
# For the license terms see the file LICENSE, distributed along with this
# software.
#
# This program is free software; you can redistribute it and/or modify it
# under the terms of the GNU General Public License as published by the
# Free Software Foundation; either version 2 of the License, or (at your
# option) any later version.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
# General Public License for more details.
#
# You should have received a copy of the GNU General Public License along
# with this program; if not, write to the Free Software Foundation Inc.,
# 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.


import numpy

import matplotlib.pyplot as plt
from matplotlib import collections, transforms
from matplotlib.colors import colorConverter


class pHexagonalMatrix():

    """
    """

    COLUMN_PITCH = 0.0500
    ROW_PITCH = 0.0433

    def __init__(self, num_columns, num_rows):
        """
        """
        self.num_columns = num_columns
        self.num_rows = num_rows
        self.__grid = []
        for col in xrange(self.num_columns):
            for row in xrange(self.num_rows):
                self.__grid.append(self.pixel2world(col, row))

    def num_pixels(self):
        """
        """
        return self.num_columns*self.num_pixels

    def pixel2world(self, col, row):
        """
        """
        x = (col - 0.5*self.num_columns + 0.5*((row + 1) % 2))*self.COLUMN_PITCH
        y = (-row + 0.5*self.num_rows)*self.ROW_PITCH
        return (x, y)

    def draw(self):
        """
        """
        fig, axes = plt.subplots(1, 1)
        poly = collections.RegularPolyCollection(6, offsets=self.__grid,
                                                 sizes=(100,),
                                                 transOffset=axes.transData)
        axes.add_collection(poly, autolim=True)
        axes.autoscale_view()
        print axes.transAxes
        print fig.transFigure
        plt.show()



matrix = pHexagonalMatrix(30, 36)
matrix.draw()


"""
pos = []
for col in xrange(num_cols):
    for row in xrange(num_rows):
        x = (col - 0.5*(row % 2))*col_pitch
        y = -row*row_pitch
        pos.append((x, y))

fig, axes = plt.subplots(1, 1)
col = collections.RegularPolyCollection(6, rotation=0., sizes=(80,),
                                        offsets=pos, transOffset=axes.transData)
#trans = transforms.Affine2D().scale(fig.dpi / 72.0)
#col.set_transform(trans)  # the points to pixels transform
axes.add_collection(col, autolim=True)
#col.set_color(colors)
axes.autoscale_view()
plt.show()
"""

if __name__ == '__main__':
    pass
