#ifndef CUSTOMHISTOGRAMPLOT_H
#define CUSTOMHISTOGRAMPLOT_H

#include <iostream>
#include <vector>

#include "qcustomplot.h"
#include "pHistogramOptions.h"

class pCustomHistogramPlot : public QCustomPlot
{
  
  Q_OBJECT
  
  public:
  
    pCustomHistogramPlot(pBasicPlotOptions options = pBasicPlotOptions());      
    void setKeyContent(double key, double value);    
    void setRange (double xmin, double xmax);
    void setTolerance (double tolerance);
    void setBinWidth (double binWidth);
    void resetBars();

  public slots:

    void updateData (const std::vector<double> &keys,
                     const std::vector<double> &values);    

  private slots:
  
    void mousePress();
    void mouseWheel();
    void selectionChanged();
      
  private:
    
    void setupInteractions();
   
    QCPBars *m_bars;
    pBasicPlotOptions m_options;    
    // Needed to properly remove key in QCPBars data
    double m_centerPosTolerance;

};

#endif  //CUSTOMHISTOGRAMPLOT_H
