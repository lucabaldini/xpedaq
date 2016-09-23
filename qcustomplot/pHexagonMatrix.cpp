#include <pHexagonMatrix.h>

void pHexagonMatrix::draw(QCustomPlot *parentPlot,
                         double xStart, double yStart,
                         unsigned int nCol, unsigned int nRow,
                         int startOdd)
{
  double padding = 0.9;
  m_hexArray.resize(nCol*nRow);
  for (unsigned int j=0; j < nRow; ++j){
    for (unsigned int i=0; i < nCol; ++i){
      double xPos = xStart + (i - 0.5 *((j+startOdd)%2)) * columnPitch();
      double yPos = yStart - j * rowPitch();
      pHexagon* hex = new pHexagon(xPos, yPos, padding*hexEdge(), parentPlot);
      m_hexArray[i+j*nCol] = hex;
    }
  }
}

double pHexagonMatrix::hexEdge()
{
  return columnPitch()/sqrt(3.);
}


double pHexagonMatrix::rowPitch()
{
  return 1.5 * hexEdge();
}


pHexagon* pHexagonMatrix::hexagon(unsigned int i)
{
  return m_hexArray.at(i);
}


void pHexagonMatrix::reset()
{
  m_hexArray.resize(0);
}
