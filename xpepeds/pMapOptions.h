#ifndef PMAPOPTIONS_H
#define PMAPOPTIONS_H

#include <QString>
#include "qcustomplot.h"

/* Small class holding the option for drawing a color map
TO DO: Implement Interactions
*/

class pMapOptions
{

public:

  pMapOptions (double xmin, double xmax, double ymin, double ymax,
               int nx, int ny, QString xtitle = "x", QString ytitle = "y",
               QString ztitle = "z", 
               QCPColorGradient colGradient = QCPColorGradient::gpHot) :
               m_xMin(xmin), m_xMax(xmax), m_yMin(ymin), m_yMax(ymax),
               m_nBinsX(nx), m_nBinsY(ny), m_xTitle(xtitle), m_yTitle(ytitle),
               m_zTitle(ztitle), m_gradientType(colGradient) {}
  
  /*TO DO: make these private and use getters/setters */
  double m_xMin;
  double m_xMax;
  double m_yMin;
  double m_yMax;
  int m_nBinsX;
  int m_nBinsY;
  QString m_xTitle;
  QString m_yTitle;
  QString m_zTitle;
  QCPColorGradient m_gradientType;

};

#endif // PMAPOPTIONS_H
