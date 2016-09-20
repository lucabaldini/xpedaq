#ifndef HISTOGRAMPLOT_H
#define HISTOGRAMPLOT_H

#include <QMutex>

#include "pCustomHistogramPlot.h"
#include "pHistogram.h"
#include "pHistogramOptions.h"

#define TOLERANCE_FACTOR  1.e-5


/* Class for drawing a 1d histogram. Inherits from pCustomHistogramPlot.
   The underlying data structure is a pHistogram object, which is
   passed as a constant to the constructor and cannot be modified in any way
   (nor deleted) inside the class.
   The class is responsible for synchronizing what is displayed on the screen
   with the content of the histogram.
*/
   
class pHistogramPlot: public pCustomHistogramPlot
{
  
  Q_OBJECT
  
  public:
       
    //Initialize from existing pHistogram
    pHistogramPlot(const pHistogram* hist, 
                   pBasicPlotOptions options = pBasicPlotOptions());
    
    unsigned int entries() const;
    double sum() const;
    void reset();

  public slots:

    /* Reset the x-axis to match the histogram dimension and set the
      optimal y-axis scale. */
    virtual void resetView();
    
    /* Synchronize the plot with the content of the pHistogram */
    void updateDisplay();
      
  private:
    
    pHistogramPlot(){;} //so it cannot be used

    const pHistogram *m_hist;
    
};

#endif  //HISTOGRAMPLOT_H
