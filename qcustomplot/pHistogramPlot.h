#ifndef HISTOGRAMPLOT_H
#define HISTOGRAMPLOT_H

#include "qcustomplot.h"
#include "pHistogram.h"
#include "pHistogramOptions.h"

class pHistogramPlot: public QCustomPlot
{
  
  Q_OBJECT
  
  public:
    
    pHistogramPlot(unsigned int nBins, double xmin, double xmax, 
                pBasicPlotOptions options = pBasicPlotOptions());
    
    unsigned int entries() const;
    double sum() const;
    void fillBin(unsigned int binNumber, double value);
    void fillBin(unsigned int binNumber);
    void fill(double x, double value);
    void fill(double x);
    void reset();

  private slots:
  
    void mousePress();
    void mouseWheel();
    void selectionChanged();    
      
  private:
    
    void setupInteractions();
    
    pHistogram *m_hist;
    QCPBars *m_bars;
    pBasicPlotOptions m_options;
    
    // Needed to properly remove key in QCPBars data
    double m_centerPosTolerance;
    
};

#endif  //PULSEHEIGHTPLOT_H
