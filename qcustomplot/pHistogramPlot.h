#ifndef HISTOGRAMPLOT_H
#define HISTOGRAMPLOT_H

#include "pCustomHistogramPlot.h"
#include "pHistogram.h"
#include "pHistogramOptions.h"

class pHistogramPlot: public pCustomHistogramPlot
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

  public slots:

    void updateData (const std::vector<double> &values);
    void drawStatisticalBox();
      
  private:
    
    pHistogram *m_hist;
    
};

#endif  //HISTOGRAMPLOT_H
