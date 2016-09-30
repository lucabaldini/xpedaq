#ifndef PHEXAGON_H
#define PHEXAGON_H

#include <cmath>
#include <QVector>
#include "qcustomplot.h"

#define PI 3.1415926535

/* Class describing a hexagon */

class pHexagon : public QCPCurve
{

  public:
  
    //Draw a regular hexagon at position (posX, posY) with edge 'size'
    pHexagon(double posX, double posY, double size, QCustomPlot *parentPlot);

};

#endif //PHEXAGON_H
