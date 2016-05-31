#include "pMapPlot.h"

pMapPlot::pMapPlot(unsigned int nXbins, double xmin, double xmax,
                   unsigned int nYbins, double ymin, double ymax,
                   pColorMapOptions options) : m_options (options)
{
  m_map = new pMap(nXbins, xmin, xmax, nYbins, ymin, ymax);
  axisRect() -> setupFullAxesBox(true);
  xAxis -> setLabel(m_options.m_xTitle);
  yAxis -> setLabel(m_options.m_yTitle);
  m_colorMap = new QCPColorMap(xAxis, yAxis);
  addPlottable(m_colorMap);
  
  // Initializing the QCPColorMapData with default values
  m_data = new QCPColorMapData(1, 1, QCPRange(0., 1.), QCPRange(0., 1.));
  // Update m_data to match the histogram binning
  setupDataMap();
  
  m_colorMap -> setData(m_data);
  m_colorMap -> setTightBoundary(false); // display full cell at the boundaries
  m_colorMap -> setInterpolate(false); //disable graphical smoothing
  
  // Do not show the grid
  xAxis -> grid() -> setSubGridVisible(false);
  yAxis -> grid() -> setSubGridVisible(false);

  m_colorScale = new QCPColorScale(this);
  plotLayout() -> addElement(0, 1, m_colorScale);
  m_colorScale -> setType(QCPAxis::atRight);
  m_colorMap -> setColorScale(m_colorScale);
  m_colorScale -> axis() -> setLabel(m_options.m_zTitle);
  m_colorMap -> setGradient(m_options.m_gradientType);
  m_colorMap -> rescaleDataRange(true);
  
  //Align thing using a margin group
  m_marginGroup = new QCPMarginGroup(this);
  axisRect() -> setMarginGroup(QCP::msBottom|QCP::msTop, m_marginGroup);
  m_colorScale -> setMarginGroup(QCP::msBottom|QCP::msTop, m_marginGroup);

  rescaleAxes();
  setupInteractions(); 
}


void pMapPlot::setupDataMap()
{
  setMacthingRange(m_map -> xMin(), m_map -> xMax(),
                   m_map -> yMin(), m_map -> yMax());
  m_data -> setSize(m_map -> nXbins(), m_map -> nYbins());
  rescaleAxes();
}


void pMapPlot::setMacthingRange(unsigned int xmin, unsigned int xmax,
                                unsigned int ymin, unsigned int ymax)
{
  /* In a QCPColorMap the first cell is centered on the lower range boundary
     and the last cell on the upper range boundary. Thus, we need to shift the
     first edge of half a bin forward and the last edge of half a bin backward,
     in order to recover a matching with the underlying histogram (i.e. to 
     have cells centered at the center of the corresponding bins).
  */
  double xPad = (m_map -> binWidthX (0,0))/2.;
  double yPad = (m_map -> binWidthY (0,0))/2.;
  QCPRange xrange = QCPRange(xmin + xPad, xmax - xPad);
  QCPRange yrange = QCPRange(ymin + yPad, ymax - yPad);
  m_data -> setRange(xrange, yrange); 
}
                              

void pMapPlot::setupInteractions()
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


void pMapPlot::selectionChanged()
{
  /* Normally, axis base line and axis tick labels are selectable separately,
     but we want the user only to be able to select the axis as a whole, so
     we tie the selected states of the tick labels and the axis base line
     together. The axis labels are not selectable.
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


void pMapPlot::mousePress()
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


void pMapPlot::mouseWheel()
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


unsigned int pMapPlot::entries() const
{
  return m_map -> entries();
}


double pMapPlot::sum() const
{
  return m_map -> sum();
}


void pMapPlot::fill(double x, double y, double value)
{ 
  unsigned int xbin, ybin;
  m_map -> findBin(x, y, xbin, ybin);
  m_map -> fill(x, y, value);
  m_data -> setData (x, y, m_map -> binContent(xbin, ybin));
  m_colorMap -> rescaleDataRange();
}


void pMapPlot::fill(double x, double y)
{
  fill(x, y, 1.);
}


void pMapPlot::setRange (unsigned int xmin, unsigned int xmax,
                         unsigned int ymin, unsigned int ymax)
{
  setMacthingRange(xmin, xmax, ymin, ymax);
  m_data -> setSize(xmax - xmin, ymax - ymin);
  rescaleAxes();
}


void pMapPlot::resetData()
{
  m_map -> reset();
  m_colorMap -> clearData();  
}


void pMapPlot::reset()
{
  resetData();
  setupDataMap();
  replot();
}

