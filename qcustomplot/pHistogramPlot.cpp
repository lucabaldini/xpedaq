#include "pHistogramPlot.h"

pHistogramPlot::pHistogramPlot(unsigned int nBins, double xmin, double xmax, 
                               pBasicPlotOptions options) :
                               pCustomHistogramPlot(options)
{
  /* TODO: as now we are using the width of the first bin as the width for
     drawing all the bars and to set the tolerance for removing keys.
     It works only under the assumption that the bins are equally spaced and
     we may need to be smarter i.e. for the case of logarithmic binning.
  */
  m_hist = new pHistogram(nBins, xmin, xmax);
  setRange(m_hist -> xMin(), m_hist -> xMax());
  setBinWidth(m_hist -> binWidth(0)); // fix this!
  setTolerance (1.e-4 * (m_hist -> binWidth(0)));  // fix this!
}


unsigned int pHistogramPlot::entries() const
{
  return m_hist -> entries();
}


double pHistogramPlot::sum() const
{
  return m_hist -> sum();
}


void pHistogramPlot::fillBin(unsigned int binIndex, double value)
{  
  // The fillBin function throws an error if the bin doesn't exist
  try
    {m_hist -> fillBin(binIndex, value);}
  catch (HistogramError histErr)
  { 
    std::cout << "Invalid bin number: " << binIndex << ". No entry filled."
              <<  std::endl;
    return;
  }
  double key = m_hist -> binCenter(binIndex);
  setKeyContent(key, m_hist -> binContent(binIndex));
}


void pHistogramPlot::fillBin(unsigned int binIndex)
{
  fillBin(binIndex, 1.);
}


void pHistogramPlot::fill(double x, double value)
{  
  //This is outside the try block so that it can fill the overflow/underflow
  m_hist -> fill(x, value); 
  
  try
  {
    unsigned int binIndex = m_hist -> findBin(x);
    double key = m_hist -> binCenter(binIndex);
    setKeyContent(key, m_hist -> binContent(binIndex));
  }
  catch (HistogramError histErr) {return;}
}


void pHistogramPlot::fill(double x)
{
  fill(x, 1.);
}


void pHistogramPlot::reset()
{
  m_hist -> reset();
  resetBars();  
}
