#include "pHistogramPlot.h"


pHistogramPlot::pHistogramPlot(unsigned int nBins, double xmin, double xmax, 
                               pBasicPlotOptions options) :
                               m_options (options)
{
  /* TODO: as now we are using the width of the first bin as the width for
     drawing all the histogram bars. It works only under the assumption that
     the bins are equally spaced and we may need to be smarter i.e. for the
     case of logarithmic binning */
  
  m_hist = new pHistogram(nBins, xmin, xmax);
  m_bars = new QCPBars(xAxis, yAxis);
  addPlottable(m_bars);
  m_bars -> keyAxis() -> setRange(m_hist -> xMin(), m_hist -> xMax());
  m_bars -> setWidth(m_hist -> binWidth(0)); // fix this!
  m_centerPosTolerance = 1.e-4 * (m_hist -> binWidth(0));  // fix this!
  xAxis -> setLabel(m_options.m_xTitle);
  yAxis -> setLabel(m_options.m_yTitle);
  setupInteractions();
}


void pHistogramPlot::setupInteractions()
{
  setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes);
  xAxis -> setSelectableParts(QCPAxis::spAxis | QCPAxis::spTickLabels);
  yAxis -> setSelectableParts(QCPAxis::spAxis | QCPAxis::spTickLabels);
  axisRect() -> setRangeZoomFactor(0.9, 0.9);
  connect(this, SIGNAL(mousePress(QMouseEvent*)), this, SLOT(mousePress()));
  connect(this, SIGNAL(mouseWheel(QWheelEvent*)), this, SLOT(mouseWheel()));
  connect(this, SIGNAL(selectionChangedByUser()),
          this, SLOT(selectionChanged()));
}


void pHistogramPlot::selectionChanged()
{
  /* Normally, axis base line and axis tick labels are selectable separately,
     but we want the user only to be able to select the axis as a whole, so
     we tie the selected states of the tick labels and the axis base line
     together. The axis labels are made not selectable.
  */
  
  if (xAxis -> selectedParts().testFlag(QCPAxis::spAxis) ||
      xAxis -> selectedParts().testFlag(QCPAxis::spTickLabels))
  {
    xAxis -> setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
  }
  
  if (yAxis -> selectedParts().testFlag(QCPAxis::spAxis) ||
      yAxis -> selectedParts().testFlag(QCPAxis::spTickLabels))
  {
    yAxis -> setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
  }
}


void pHistogramPlot::mousePress()
{
  /* If an axis is selected, only allow the direction of that axis to be
     dragged.
     If no axis is selected, both directions may be dragged */
  
  if (xAxis->selectedParts().testFlag(QCPAxis::spAxis))
    {axisRect()->setRangeDrag(xAxis->orientation());}
  else if (yAxis->selectedParts().testFlag(QCPAxis::spAxis))
    {axisRect()->setRangeDrag(yAxis->orientation());}
  else
    {axisRect()->setRangeDrag(Qt::Horizontal|Qt::Vertical);}
}


void pHistogramPlot::mouseWheel()
{
  /* If an axis is selected, only allow the direction of that axis to be
     zoomed.
     If no axis is selected, both directions may be zoomed */
  
  if (xAxis->selectedParts().testFlag(QCPAxis::spAxis))
    {axisRect()->setRangeZoom(xAxis->orientation());}
  else if (yAxis->selectedParts().testFlag(QCPAxis::spAxis))
    {axisRect()->setRangeZoom(yAxis->orientation());}
  else
    {axisRect()->setRangeZoom(Qt::Horizontal|Qt::Vertical);}
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
  /* QCPBars does not provide a method for incrementing the content of a bin.
     The default beahviour, when the function addData() is called on
     a key which already exists, is to create another entry for the same key.
     So we need to remove the old value and recreate the pair key:value with
     the updated content.
  */ 
  
  // The fillBin function thorws an error if the bin doesn't exist
  try
    {m_hist -> fillBin(binIndex, value);}
  catch (HistogramError histErr)
  { 
    std::cout << "Invalid bin number: " << binIndex << ". No entry filled."
              <<  std::endl;
    return;
  }
  double key = m_hist -> binCenter(binIndex);
  /* We use removeData() on a small interval centered around the key value
     to make sure we actually remove it */
  m_bars -> removeData(key - m_centerPosTolerance, key + m_centerPosTolerance);
  m_bars -> addData(key, m_hist -> binContent(binIndex));
  m_bars -> rescaleValueAxis();
}

void pHistogramPlot::fillBin(unsigned int binIndex)
{
  fillBin(binIndex, 1.);
}


void pHistogramPlot::fill(double x, double value)
{
  /* QCPBars does not provide a method for incrementing the content of a bin.
     The default beahviour, when the function addData() is called on
     a key which already exists, is to create another entry for the same key.
     So we need to remove the old value and recreate the pair key:value with
     the updated content.
  */
  
  //This is outside the try block so that it fills the overflow/underflow
  m_hist -> fill(x, value); 
  
  try
  {
    unsigned int binIndex = m_hist -> findBin(x);
    double key = m_hist -> binCenter(binIndex);
    /* We use removeData() on a small interval centered around the key value
     to make sure we actually remove it */
    m_bars -> removeData(key - m_centerPosTolerance,
                         key + m_centerPosTolerance);
    m_bars -> addData(key, m_hist -> binContent(binIndex));
  }
  catch (HistogramError histErr) {return;}
  m_bars -> rescaleValueAxis();
}


void pHistogramPlot::fill(double x)
{
  fill(x, 1.);
}


void pHistogramPlot::reset()
{
  m_hist -> reset();
  m_bars -> clearData();
  replot();  
}
