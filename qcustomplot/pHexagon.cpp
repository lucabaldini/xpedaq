#include "pHexagon.h"
#include <iostream>

//see http://www.redblobgames.com/grids/hexagons/

pHexagon::pHexagon(double posX, double posY, double size,
                   QCustomPlot *parentPlot):
                   QCPCurve(parentPlot->xAxis, parentPlot->yAxis)
{
  const int nPoints = 7; // we need an extra point to close the curve
  QVector<double> vertexIndex(nPoints);
  QVector<double> xCoord(nPoints);
  QVector<double> yCoord(nPoints);
  for (int i =0; i < nPoints; ++i){
      double angleDeg = 60. * i   + 30.;
      double angleRad = (PI / 180.) * angleDeg;
      vertexIndex[i] = i;
      xCoord[i] = posX + size * cos(angleRad);
      yCoord[i] = posY + size * sin(angleRad);
  }
  setData (vertexIndex, xCoord, yCoord);
  setLineStyle(QCPCurve::lsLine);
  setPen(QPen(Qt::black));
  setScatterStyle(QCPScatterStyle::ssDot);
  parentPlot->addPlottable(this);
}
