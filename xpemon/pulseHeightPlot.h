#ifndef PULSEHEIGHTPLOT_H
#define PULSEHEIGHTPLOT_H

#include <vector>
#include <math.h>
#include "qcustomplot.h"

class PulseHeightPlot: public QCustomPlot
{
  public:
    
    PulseHeightPlot(unsigned int nBins = 20,
                    double xmin = 0., double xmax = 200.);
    void addPoint(int pHeight);
  
  private:
    
    double binWidth();
    double binCenter(unsigned int bin);
    unsigned int findBin(double value);
    
    QCPBars *m_pulseHeightHist;
    unsigned int m_nBins;
    double m_xmin;
    double m_xmax;
    std::vector<double> m_values;
    std::vector<double> m_keys; //Not really used at the moment
    
};

#endif  //PULSEHEIGHTPLOT_H
