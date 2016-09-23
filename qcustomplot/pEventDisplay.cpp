#include "pEventDisplay.h"

pEventDisplay::pEventDisplay(pColorMapOptions options) : m_options(options)
{
  //Initialize window boundaries
  m_colMin = 0;
  m_colMax = 0;
  m_rowMin = 0;
  m_rowMax = 0;
  
  //Initialize axes with default range
  axisRect()->setupFullAxesBox(true);
  xAxis->setRange(0., 1.);
  xAxis->setLabel(m_options.m_xTitle);
  yAxis->setRange(0., 1.); 
  yAxis->setLabel(m_options.m_yTitle);
    
  //Do not show the grid
  xAxis -> grid() -> setSubGridVisible(false);
  yAxis -> grid() -> setSubGridVisible(false);
  
  //Initialize the color scale:
  m_dataRange = QCPRange(0., 1.);
  m_colorScale = new QCPColorScale(this);
  plotLayout()->addElement(0, 1, m_colorScale);
  m_colorScale->setType(QCPAxis::atRight);
  m_colorScale->axis()->setLabel(m_options.m_zTitle);
  m_colorScale->setDataRange(m_dataRange);
  m_colorScale->setGradient(m_options.m_gradientType);
  m_colorScale -> axis() -> setLabel(m_options.m_zTitle);
  
  //Align things using a margin group:
  m_marginGroup = new QCPMarginGroup(this);
  axisRect()->setMarginGroup(QCP::msBottom|QCP::msTop, m_marginGroup);
  m_colorScale->setMarginGroup(QCP::msBottom|QCP::msTop, m_marginGroup);
  
  //Initialize the matrix
  m_hexMatrix = new pHexagonMatrix(P_C);  
  m_isSyncronized = true;
  
  setupInteractions();
}


void pEventDisplay::setWindowRange (unsigned int firstCol, unsigned int lastCol,
                                unsigned int firstRow, unsigned int lastRow)
{
  m_colMin = firstCol;
  m_colMax = lastCol;
  m_rowMin = firstRow;
  m_rowMax = lastRow;
}


void pEventDisplay::setDataRange (const QCPRange &dataRange)
{
  if (m_dataRange.lower != dataRange.lower ||
      m_dataRange.upper != dataRange.upper) {
    m_dataRange = dataRange;
    emit dataRangeChanged(m_dataRange);
  }
  if (m_isSyncronized){
    updateMatrixColor();
    replot();
  }
}


void pEventDisplay::setAdcData(const std::vector<double> &values)
{
  m_AdcCounts = values; //will automatically resize m_AdcCounts if necessary
  m_isSyncronized = false;
  updateDataRange();
}


void pEventDisplay::updateDataRange()
{
  double xmin = 0;
  double xmax = 0;
  for (auto it = m_AdcCounts.begin(); it!= m_AdcCounts.end(); ++it){
    if ((*it) < xmin) xmin = (*it);
    if ((*it) > xmax) xmax = (*it);
  }
  setDataRange(QCPRange(xmin, xmax));
}


void pEventDisplay::updateAxesRange()
{
  double xmin, xmax, ymin, ymax;
  //Note that y is decreasing with row number. (0,0) is left uppermost.
  pixelToCoord(m_colMin, m_rowMax, xmin, ymin);
  pixelToCoord(m_colMax, m_rowMin, xmax, ymax);
  double maxDim = std::max(xmax - xmin, ymax - ymin);
  double padX = 0.5*(maxDim - (xmax - xmin));
  double padY = 0.5*(maxDim - (ymax - ymin));
  xAxis->setRange(xmin - padX - P_C, xmax + padX + P_C);
  yAxis->setRange(ymin - padY - P_C, ymax + padY + P_C);
}


void pEventDisplay::updateColorScale()
{
  m_colorScale->setDataRange(m_dataRange);
}


void pEventDisplay::updateMatrixColor()
{
  int nData = m_AdcCounts.size();
  QRgb* scanLine = new QRgb[nData];
  QCPColorGradient gradient = QCPColorGradient(m_options.m_gradientType);   
  gradient.colorize (m_AdcCounts.data(), m_dataRange, scanLine, nData); 
  for (int i=0; i<nData ;++i) {
    QColor color (scanLine[i]);
    m_hexMatrix -> hexagon(i) -> setBrush(QBrush(color));
  }
}


void pEventDisplay::drawMatrix()
{
  double xmin, ymin;
  pixelToCoord(m_colMin, m_rowMin, xmin, ymin);
  m_hexMatrix->draw(this, xmin, ymin, m_colMax - m_colMin + 1,
                    m_rowMax - m_rowMin + 1, m_colMin%2);
  m_isSyncronized = true;
}


void pEventDisplay::draw()
{
  clearPlottables();
  updateAxesRange();
  drawMatrix();
  updateMatrixColor();
  replot();
}



void pEventDisplay::resetView()
{
  clearPlottables();
  updateAxesRange();
  drawMatrix();
  updateDataRange();
}


void pEventDisplay::clearMap()
{  
  clearPlottables();
  m_hexMatrix->reset();
  replot();
}


void pEventDisplay::pixelToCoord(unsigned int i, unsigned int j,
                             double &x, double &y)
{
  x = (i - 0.5 * (300 - 1.5 + j%2 )) * (m_hexMatrix->columnPitch());
  y = (0.5 * (352 - 1) - j) * (m_hexMatrix->rowPitch());
}

 
void pEventDisplay::setupInteractions()
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

  // Keep the color scale synchronized with the plot
  connect(this, SIGNAL(dataRangeChanged(QCPRange)),
          m_colorScale, SLOT(setDataRange(QCPRange)));
  //connect(this, SIGNAL(dataScaleTypeChanged(QCPAxis::ScaleType)),
  //        m_colorScale, SLOT(setDataScaleType(QCPAxis::ScaleType)));
  //connect(this, SIGNAL(gradientChanged(QCPColorGradient)),
  //        m_colorScale, SLOT(setGradient(QCPColorGradient)));
  connect(m_colorScale, SIGNAL(dataRangeChanged(QCPRange)),
          this, SLOT(setDataRange(QCPRange)));
  //connect(m_colorScale, SIGNAL(gradientChanged(QCPColorGradient)),
  //        this, SLOT(setGradient(QCPColorGradient)));
  //connect(m_colorScale, SIGNAL(dataScaleTypeChanged(QCPAxis::ScaleType)),
  //        this, SLOT(setDataScaleType(QCPAxis::ScaleType)));
}
  


void pEventDisplay::selectionChanged()
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


void pEventDisplay::mousePress()
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


void pEventDisplay::mouseWheel()
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


void pEventDisplay::contextMenuRequest(QPoint pos)
{
  QMenu *menu = new QMenu(this);
  menu->setAttribute(Qt::WA_DeleteOnClose);
  menu->addAction("Restore initial view", this, SLOT(resetView()));
  menu->popup(mapToGlobal(pos));
}
