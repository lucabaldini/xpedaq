#ifndef HISTOGRAMPLOT_H
#define HISTOGRAMPLOT_H

#include <QMutex>

#include "pCustomHistogramPlot.h"
#include "pHistogram.h"
#include "pHistogramOptions.h"

#define TOLER_FACTOR  1.e-5


/* Class handling the interactions (drawing and data access) of a 1d
   histogram. Inherits from pCustomHistogramPlot.
   The underlying data structure is a pHistogram object, which may be created
   with the object or pre-existant. The class is responsible for keeping
   synchronized the pHistogram and the pCustomHistogramPlot data content.
*/
   
class pHistogramPlot: public pCustomHistogramPlot
{
  
  Q_OBJECT
  
  public:
    
    //Construct from scracth, creating its own underlying pHistorgram
    pHistogramPlot(unsigned int nBins, double xmin, double xmax, 
                   pBasicPlotOptions options = pBasicPlotOptions());
    //Initialize from existing pHistogram (shared ownership)
    pHistogramPlot(pHistogram* hist, 
                   pBasicPlotOptions options = pBasicPlotOptions());
    
    unsigned int entries() const;
    double sum() const;
    
    void fillBin(unsigned int binNumber, double value);
    void fillBin(unsigned int binNumber);
    void fill(double x, double value);
    void fill(double x);
    void reset();

  public slots:

    /* Reset the x-axis to match the histogram dimension and set the
      optimal y-axis scale. */
    virtual void resetView();
    
    void updateData (const std::vector<double> &values);
      
  private:
    
    pHistogram *m_hist;
    
};

#endif  //HISTOGRAMPLOT_H
