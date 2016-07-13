#include "pHistogramPlot.h"

pHistogramPlot::pHistogramPlot(unsigned int nBins, double xmin, double xmax, 
                               pBasicPlotOptions options) :
                               pCustomHistogramPlot(options)
{
  /* TODO: as now we are assuming equally spaced bins for setting the width
     of the graphical bars and the tolerance for removing keys. We may need to
     be smarter, i.e. to handle the case of logarithmic binning.
  */
  m_hist = new pHistogram(nBins, xmin, xmax);
  m_bars -> keyAxis() -> setRange(m_hist -> xMin(), m_hist -> xMax());
  m_bars -> setWidth(m_hist -> binWidth());
  setTolerance (1.e-4 * (m_hist -> binWidth()));
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
    std::cout << "Invalid bin number: " << binIndex
              << ". No entry filled." <<  std::endl;
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


void pHistogramPlot::updateData (const std::vector<double> &values)
{
  /* This is a fast (and unsafe) method for refilling the histogram.
     It assumes that the input vector has the correct size.
     WARNING: no check is performed.
  */
  m_hist -> reset();
  for (unsigned int i = 0; i < values.size(); ++i)
    {if (values.at(i) > 0) fillBin(i, values.at(i));}
}


void pHistogramPlot::resetView()
{
  m_bars -> keyAxis() -> setRange(m_hist -> xMin(), m_hist -> xMax());
  m_bars -> rescaleValueAxis();
  replot();
}


/*
void pHistogramPlot::setupStatBox()
{
  m_statLabel = new QCPItemText(this);
  addItem(m_statLabel);
  // add the text label at the top:
  m_statLabel -> setPositionAlignment(Qt::AlignTop|Qt::AlignHCenter);
  // place position at center/top of axis rect
  m_statLabel -> position -> setType(QCPItemPosition::ptAxisRectRatio);   
  m_statLabel -> position -> setCoords(0.5, 0);
  // make font a bit larger  
  m_statLabel -> setFont(QFont(font().family(), 10));
  // show black border around text
  m_statLabel -> setPen(QPen(Qt::black)); 
}
*/


void pHistogramPlot::reset()
{
  m_hist -> reset();
  clearBars();
}
