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
