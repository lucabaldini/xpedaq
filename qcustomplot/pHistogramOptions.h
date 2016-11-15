#ifndef PHISTOGRAMOPTIONS_H
#define PHISTOGRAMOPTIONS_H

#include <QString>
#include "qcustomplot.h"

/* Base class for passing draw options to the plots */

class pBasicPlotOptions
{

public: 
  
  pBasicPlotOptions (QString plotName = "", QString xtitle = "x",
                     QString ytitle = "y", QPen pen = QPen(),
                     QBrush brush = QBrush()) :
                     m_plotName (plotName), m_xTitle(xtitle),
                     m_yTitle(ytitle), m_pen (pen), m_brush (brush)
                     {};

  QString m_plotName;
  QString m_xTitle;
  QString m_yTitle;
  QPen m_pen;
  QBrush m_brush;
               
};

/* Small class holding the option for drawing a color map
TO DO: Implement Interactions
*/

class pColorMapOptions : public pBasicPlotOptions
{

public:

  pColorMapOptions (QString plotName = "", QString xtitle = "x",
                    QString ytitle = "y", QString ztitle = "z", 
                    QCPColorGradient colGradient =
                                               QCPColorGradient::gpThermal,
                    bool colorScale = true,
                    bool drawFullAxesBox = true) :
                    pBasicPlotOptions(plotName, xtitle, ytitle),
                    m_zTitle(ztitle), m_gradientType(colGradient),
                    m_colorScale (colorScale),
                    m_drawFullAxesBox (drawFullAxesBox) {};

  QString m_zTitle;
  QCPColorGradient m_gradientType;
  bool m_colorScale; // show the color scale on the right
  bool m_drawFullAxesBox; // allow automatic set-up of all the axes around the plot
};

#endif // PHISTOGRAMOPTIONS_H
