#include "pCustomColorMapPlot.h"

pCustomColorMapPlot::pCustomColorMapPlot(pColorMapOptions options) : 
                                                            m_options(options)
{
  axisRect() -> setupFullAxesBox(true);
  xAxis -> setLabel(m_options.m_xTitle);
  yAxis -> setLabel(m_options.m_yTitle);
  m_colorMap = new QCPColorMap(xAxis, yAxis);
  addPlottable(m_colorMap);
  
  // Initializing the QCPColorMapData with default values
  m_data = new QCPColorMapData(1, 1, QCPRange(0., 1.), QCPRange(0., 1.));
  
  m_colorMap -> setData(m_data);
  m_colorMap -> setTightBoundary(false); //display full cell at the boundaries
  m_colorMap -> setInterpolate(false); //disable graphical smoothing
  
  // Do not show the grid
  xAxis -> grid() -> setSubGridVisible(false);
  yAxis -> grid() -> setSubGridVisible(false);

  // Initialize the color scale
  m_colorScale = new QCPColorScale(this);
  plotLayout() -> addElement(0, 1, m_colorScale);
  m_colorScale -> setType(QCPAxis::atRight);
  m_colorMap -> setColorScale(m_colorScale);
  m_colorScale -> axis() -> setLabel(m_options.m_zTitle);
  m_colorMap -> setGradient(m_options.m_gradientType);
  m_colorMap -> rescaleDataRange(true);
  
  //Align things using a margin group
  m_marginGroup = new QCPMarginGroup(this);
  axisRect() -> setMarginGroup(QCP::msBottom|QCP::msTop, m_marginGroup);
  m_colorScale -> setMarginGroup(QCP::msBottom|QCP::msTop, m_marginGroup);

  rescaleAxes();
  setupInteractions(); 
}


void pCustomColorMapPlot::setCellContent(unsigned int xCell,
                                         unsigned int yCell,
                                         double value)
{
  m_data -> setCell (xCell, yCell, value);
  m_colorMap -> rescaleDataRange();
}

void pCustomColorMapPlot::setDataContent(double x, double y, double value)
{
  m_data -> setData (x, y, value);
  m_colorMap -> rescaleDataRange();
}


void pCustomColorMapPlot::updateData (const std::vector<double> &values)
{
  /* This is a fast (and unsafe) method for filling the color map.
     It assumes that the input vector has the correct size and follows the
     same ordering logic as a pMap::m_values.
  */
  for (unsigned int iy = 0; iy < m_data -> valueSize(); ++iy)
  {
    for (unsigned int ix = 0; ix < m_data -> keySize(); ++ix)
      {setCellContent(ix, iy, values.at(ix + iy * m_data -> keySize()));}
  }
  rescaleAxes();
}

void pCustomColorMapPlot::resetView()
{
  m_colorMap -> rescaleAxes();
  m_colorMap -> rescaleDataRange();
  replot();
}


void pCustomColorMapPlot::setRange (double xmin, double xmax,
                                    double ymin, double ymax)
{
  m_data -> setRange(QCPRange(xmin, xmax), QCPRange(ymin, ymax));
  rescaleAxes();
}


void pCustomColorMapPlot::clearMap()
{
  m_colorMap -> clearData();
  replot();
}


void pCustomColorMapPlot::setupInteractions()
{
  /* Activate interactions for axis:
     - Dragging
     - Zooming
     - Selecting
  */
  setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes);
  
  /* Set the selectable parts of the axis object:
     - axis base line
     - axis tick labels 
  */
  xAxis -> setSelectableParts(QCPAxis::spAxis | QCPAxis::spTickLabels);
  yAxis -> setSelectableParts(QCPAxis::spAxis | QCPAxis::spTickLabels);

  // Set the quickness of the zooming
  axisRect() -> setRangeZoomFactor(0.9, 0.9);
  
  // Activate connections for mouse actions
  connect(this, SIGNAL(mousePress(QMouseEvent*)), this, SLOT(mousePress()));
  connect(this, SIGNAL(mouseWheel(QWheelEvent*)), this, SLOT(mouseWheel()));
  connect(this, SIGNAL(selectionChangedByUser()),
          this, SLOT(selectionChanged()));
  
  // Setup policy and connect slot for context menu popup:        
  setContextMenuPolicy(Qt::CustomContextMenu);
  connect(this, SIGNAL(customContextMenuRequested(QPoint)),
          this, SLOT(contextMenuRequest(QPoint)));
}


void pCustomColorMapPlot::selectionChanged()
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


void pCustomColorMapPlot::mousePress()
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


void pCustomColorMapPlot::mouseWheel()
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


void pCustomColorMapPlot::contextMenuRequest(QPoint pos)
{
  QMenu *menu = new QMenu(this);
  menu->setAttribute(Qt::WA_DeleteOnClose);
  menu->addAction("Restore initial view", this, SLOT(resetView()));
  menu->popup(mapToGlobal(pos));
}
