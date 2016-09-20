#include "pHistogramPlot.h"

pHistogramPlot::pHistogramPlot(const pHistogram* hist, 
                               pBasicPlotOptions options) :
                               pCustomHistogramPlot(options),
                               m_hist(hist)
{
  m_bars -> keyAxis() -> setRange(m_hist -> xMin(), m_hist -> xMax());
  m_bars -> setWidth(m_hist -> binWidth());
  setTolerance (TOLERANCE_FACTOR * (m_hist -> binWidth()));
}


unsigned int pHistogramPlot::entries() const
{
  return m_hist -> entries();
}


double pHistogramPlot::sum() const
{
  return m_hist -> sum();
}


void pHistogramPlot::resetView()
{
  m_bars -> keyAxis() -> setRange(m_hist -> xMin(), m_hist -> xMax());
  m_bars -> rescaleValueAxis();
  replot();
}


void pHistogramPlot::updateDisplay()
{
  for (unsigned int binIndex=0; binIndex < (m_hist -> nbins()); ++binIndex){
      double key = m_hist -> binCenter(binIndex);
      setKeyContent(key, m_hist -> binContent(binIndex));
  }
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
  //m_hist -> reset();
  clearBars();
}

