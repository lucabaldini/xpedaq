#ifndef PMAPOPTIONS_H
#define PMAPOPTIONS_H

#include <QString>
#include "qcustomplot.h"

/* Base class for passing draw options to plots */

class pBasicPlotOptions
{

public: 
  
  QString m_xTitle;
  QString m_yTitle;
  
  pBasicPlotOptions (QString xtitle = "x", QString ytitle = "y") :
                     m_xTitle(xtitle), m_yTitle(ytitle) {};
               
};

/* Small class holding the option for drawing a color map
TO DO: Implement Interactions
*/

class pColorMapOptions : public pBasicPlotOptions
{

public:

  QString m_zTitle;
  QCPColorGradient m_gradientType;


  pColorMapOptions (QString xtitle = "x", QString ytitle = "y",
                    QString ztitle = "z", 
                    QCPColorGradient colGradient = QCPColorGradient::gpHot) :
                    pBasicPlotOptions(xtitle, ytitle), m_zTitle(ztitle),
                    m_gradientType(colGradient) {};

};

#endif // PMAPOPTIONS_H
