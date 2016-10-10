/***********************************************************************
Copyright (C) 2007--2016 the X-ray Polarimetry Explorer (XPE) team.

For the license terms see the file LICENSE, distributed along with this
software.

This program is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation; either version 2 of the License, or (at your
option) any later version.

This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the Free Software Foundation Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
***********************************************************************/

#ifndef HEXAGONMATRIX_H
#define HEXAGONMATRIX_H

#include <vector>

#include "pHexagon.h"
#include "qcustomplot.h"


/* 2D matrix of horizontally contiguos regular hexagons (pointy topped).
   All the dimensions are determined by the distance between the centers
   of two adjacent hexagons in a row
*/

class pHexagonMatrix
{
  public:
    
    pHexagonMatrix(double colPitch) : m_columnPitch(colPitch),
                                      m_xStart(0.),
                                      m_yStart(0.),
                                      m_nCol(0),
                                      m_nRow(0),
                                      m_firstLeft(false),
                                      m_padding(0.) {;}
    
    //Draw a matrix nCol*nRow starting from the left uppermost pixel 
    void draw (QCustomPlot *parentPlot, double xStart, double yStart,
               int nCol, int nRow, bool firstLeft = false,
               double padding = 0.);
    void reset();
    
    //getters
    pHexagon* hexagon(int index);
    double columnPitch(){return m_columnPitch;}
    double rowPitch();
    double hexEdge();
    int nRow(){return m_nRow;}
    int nColumn(){return m_nCol;}
    bool border(int index) const; 
    
    //iterator
    typedef std::vector<pHexagon*>::iterator matrixIterator;
    typedef std::vector<pHexagon*>::const_iterator const_matrixIterator;
    matrixIterator begin() {return m_hexArray.begin();}
    matrixIterator end() {return m_hexArray.end();}
    
  private:

    double m_columnPitch;
    double m_xStart;
    double m_yStart;
    int m_nCol;
    int m_nRow;
    bool m_firstLeft; // first row is left shifted (odd rows in xpe)
    std::vector<pHexagon*> m_hexArray;
    double m_padding;
};

#endif //HEXAGONMATRIX_H
