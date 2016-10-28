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

#include <pHexagonMatrix.h>

void pHexagonMatrix::draw(QCustomPlot *parentPlot,
                         double xStart, double yStart,
                         int nCol, int nRow,
                         bool firstLeft, double padding)
{
  m_xStart = xStart;
  m_yStart = yStart;
  m_nCol = nCol;
  m_nRow = nRow;
  m_firstLeft = firstLeft;
  m_padding = padding;
  m_hexArray.resize(nCol * nRow);
  /* Since the hexagon coordinates are connected to the axes, we need to
  handle the case of reverse oriented axes (value in decreasing order) */
  bool xIsReverted = parentPlot->xAxis->rangeReversed();
  int xOrient = (-2) * xIsReverted + 1; //-1 reverted, 1 normal
  bool yIsReverted = parentPlot->yAxis->rangeReversed();
  int yOrient = (-2) * yIsReverted + 1; //-1 reverted, 1 normal
  for (int j=0; j < nRow; ++j){
    for (int i=0; i < nCol; ++i){
      double xPos = xStart + xOrient * (i - 0.5 *((j + firstLeft)%2))
                    * columnPitch();
      double yPos = yStart -  yOrient * (j * rowPitch());
      pHexagon* hex = new pHexagon(xPos, yPos, (1. - padding) * hexEdge(),
                                   parentPlot);
      m_hexArray[i + (j * nCol)] = hex;
    }
  }
}


bool pHexagonMatrix::border(int index) const
{
  return (index % m_nCol == 0) || (index % m_nCol == m_nCol - 1) ||
    (index / m_nCol == 0) || (index / m_nCol == m_nRow - 1);
}

double pHexagonMatrix::hexEdge()
{
  return 0.57735026919*columnPitch(); // edge = col_pitch * 3**(-1/2)
}


double pHexagonMatrix::rowPitch()
{
  return 1.5 * hexEdge();  // row_pitch = 3/2 * col_pitch
}


pHexagon* pHexagonMatrix::hexagon(int i)
{
  return m_hexArray.at(i);
}


void pHexagonMatrix::reset()
{
  m_hexArray.resize(0);
}


double pHexagonMatrix::selectTest(const QPointF &pos, bool onlySelectable,
                                  QVariant* details) const
{
  double result = 0.;
  for (auto const &hex: m_hexArray){
    double _test =  hex->selectTest(pos, onlySelectable, details);
    if (_test > result) result = _test; //any non zero will do
  }
  return result;
}
