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


class pHexagonalMatrix():

    """
    """

    COLUMN_PITCH = 0.0500
    ROW_PITCH = 0.0433

    def __init__(self, num_columns, num_rows, start_column=0, start_row=0):
        """
        """
        self.num_columns = num_columns
        self.num_rows = num_rows
        self.start_column = start_column
        self.start_row = start_row
        self.__grid = []
        for col in xrange(start_column, start_column + num_columns):
            for row in xrange(start_row, start_row + num_rows):
                self.__grid.append(self.pixel2world(col, row))

    def grid(self):
        """
        """
        return self.__grid

    def pixel2world(self, col, row):
        """Convert from pixel coordinates to world coordinates.

        This is using the DAQ coordinate system, where the origin is in the
        top-left corner of the array, the x coordinate is spanning the columns
        from left to right and the y coordinate is spanning the rows from top
        to bottom.
        """
        x = (col - 0.5*((row + 1) % 2) + 0.5)*self.COLUMN_PITCH
        y = -row*self.ROW_PITCH
        return (x, y)

    def frame(self, padding=0.1):
        """Return a (xmin, ymin, xmax, ymax) containing the entire matrix.
        """
        xmin, ymin = self.pixel2world(self.start_column,
                                      self.start_row + self.num_rows - 1)
        xmax, ymax = self.pixel2world(self.start_column + self.num_columns - 1,
                                      self.start_row)
        # Make sure we include the pixel edges.
        xmin -= 0.5*self.COLUMN_PITCH
        xmax += 0.5*self.COLUMN_PITCH
        ymin -= 0.5*self.ROW_PITCH
        ymin += 0.5*self.ROW_PITCH
        # Add the padding.
        dx = padding*(xmax - xmin)
        dy = padding*(ymax - ymin)
        xmin -= dx
        xmax += dx
        ymin -= dy
        ymax += dy
        return xmin, ymin, xmax, ymax

    def draw(self, colors='white', show=True):
        """
        """
        xmin, ymin, xmax, ymax = self.frame()
        fig = plt.figure(figsize=(10, 10), dpi=80, facecolor='w')
        ax = plt.subplot(111, aspect='equal', adjustable='box-forced')
        ax.set_xlim([xmin, xmax])
        ax.set_ylim([ymin, ymax])
        # Overall average canvas dimensions in pixels.
        dim = (ax.transData.transform((xmin, ymin)) -
               ax.transData.transform((xmax, ymax))).mean()
        # Calculate a something proportional to the hexagon area in px**2. 
        dim = (dim/(xmax-xmin)*0.85*self.COLUMN_PITCH)**2        
        poly = collections.RegularPolyCollection(6,
                                                 offsets=self.__grid,
                                                 sizes=(dim,),
                                                 transOffset=ax.transData,
                                                 facecolors=colors,
                                                 edgecolors='gray')
        ax.add_collection(poly, autolim=True)
        plt.grid()

        def _htxt(x, y, s, **kwargs):
            """
            """
            plt.text(x, y + 0.05, '%s' % s,
                     horizontalalignment='center',
                     verticalalignment='center')

        def _vtxt(x, y, s, **kwargs):
            """
            """
            plt.text(x - 0.05, y, '%s' % s,
                     horizontalalignment='right',
                     verticalalignment='center')  

        x1, y1 = self.__grid[0]
        x2, y2 = self.__grid[-2]
        x3, y3 = self.__grid[-1]
        _htxt(x1, y1, self.start_column)
        _htxt(x2, y1, self.start_column + self.num_columns - 1)
        _vtxt(x1, y1, self.start_row)
        _vtxt(x1, y3, self.start_row + self.num_rows - 1)
        plt.xlabel('DAQ x [mm]')
        plt.ylabel('DAQ y [mm]')
        if show:
            plt.show()



if __name__ == '__main__':
    matrix = pHexagonalMatrix(30, 36, 10, 15)
    matrix.draw()

