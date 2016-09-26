#include "pEventDisplay.h"

pEventDisplay::pEventDisplay(pColorMapOptions options) : m_options(options)
{
  //Initialize window boundaries
  m_colMin = 0;
  m_colMax = 0;
  m_rowMin = 0;
  m_rowMax = 0;
  
  //Initialize axes with default range
  axisRect()->setupFullAxesBox(false);

  xAxis->setRange(-7.4875, 7.4875);
  xAxis->setLabel(m_options.m_xTitle);
  
  yAxis->setRange(-7.5991, 7.5991); 
  yAxis->setLabel(m_options.m_yTitle);
    
  xAxis2->setRange(0, 300);
  xAxis2->setLabel("column number");
  xAxis2->setNumberFormat("f");
  xAxis2->setNumberPrecision(0); // no decimal digits for integer index
  xAxis2->setVisible(true);
  xAxis2->setTickLabels(true);
  
  yAxis2->setRange(0, 352);
  yAxis2->setLabel("row number");
  yAxis2->setNumberFormat("f");
  yAxis2->setNumberPrecision(0);  // no decimal digits for integer index
  yAxis2->setRangeReversed(true);
  yAxis2->setVisible(true);
  yAxis2->setTickLabels(true);  

  //Do not show the grid
  xAxis->grid()->setSubGridVisible(false);
  yAxis->grid()->setSubGridVisible(false);
  
  //Initialize the color scale:
  m_dataRange = QCPRange(0., 1.);
  m_colorScale = new QCPColorScale(this);
  plotLayout()->addElement(0, 1, m_colorScale);
  m_colorScale->setType(QCPAxis::atRight);
  m_colorScale->axis()->setLabel(m_options.m_zTitle);
  m_colorScale->setDataRange(m_dataRange);
  m_colorScale->setGradient(m_options.m_gradientType);
  m_colorScale->axis()->setLabel(m_options.m_zTitle);
  
  //Align things using a margin group:
  m_marginGroup = new QCPMarginGroup(this);
  axisRect()->setMarginGroup(QCP::msBottom|QCP::msTop, m_marginGroup);
  m_colorScale->setMarginGroup(QCP::msBottom|QCP::msTop, m_marginGroup);
  
  //Initialize the matrix
  m_hexMatrix = new pHexagonMatrix(P_C);  
  m_isSyncronized = true;
  
  setupInteractions();
}


void pEventDisplay::setWindowRange (int firstCol,
                                    int lastCol,
                                    int firstRow,
                                    int lastRow)
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
  double zmin = 0;
  double zmax = 0;
  for (auto it = m_AdcCounts.begin(); it!= m_AdcCounts.end(); ++it){
    if ((*it) < zmin) zmin = (*it);
    if ((*it) > zmax) zmax = (*it);
  }
  setDataRange(QCPRange(zmin, zmax));
}


void pEventDisplay::updateAxesRange()
{
  int maxRange = std::max(m_rowMax - m_rowMin + 1, m_colMax - m_colMin + 1);
  int halfColExtension = (maxRange - (m_colMax - m_colMin + 1))/2;
  int halfRowExtension = (maxRange - (m_rowMax - m_rowMin + 1))/2;
  int wideColMin = m_colMin - halfColExtension - 1;
  int wideColMax = m_colMax + halfColExtension + 1;
  int wideRowMin = m_rowMin - halfRowExtension - 1;
  int wideRowMax = m_rowMax + halfRowExtension + 1;
  double xmin, xmax, ymin, ymax;
  //Note that y is decreasing with row number, so (0,0) is left uppermost. 
  pixelToCoord(wideColMin, wideRowMax, xmin, ymin);
  pixelToCoord(wideColMax, wideRowMin, xmax, ymax);
  xAxis->setRange(xmin, xmax);
  yAxis->setRange(ymin, ymax);
  xAxis2->setRange(wideColMin, wideColMax);
  yAxis2->setRange(wideRowMin, wideRowMax);  
}


void pEventDisplay::colAxisUpdate(QCPRange range)
{
  int row, colMin, colMax;
  coordToPixel(range.lower, 0., colMin, row);
  coordToPixel(range.upper, 0., colMax, row);
  xAxis2->setRange(colMin, colMax);
}


void pEventDisplay::rowAxisUpdate(QCPRange range)
{
  int col, rowMin, rowMax;
  coordToPixel(0., range.lower, col, rowMin);
  coordToPixel(0., range.upper, col, rowMax);
  yAxis2->setRange(rowMin, rowMax);
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
  double xmin, xmax, ymin, ymax;
  pixelToCoord(m_colMin, m_rowMin, xmin, ymax);
  pixelToCoord(m_colMax, m_rowMax, xmax, ymin);
  m_hexMatrix->draw(this, xmin, ymax, m_colMax - m_colMin + 1,
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


void pEventDisplay::pixelToCoord(int i, int j, double &x, double &y)
{
  x = (i - 0.5 * (298.5 + j%2 )) * (m_hexMatrix->columnPitch());
  y = (175.5 - j) * (m_hexMatrix->rowPitch());
}


void pEventDisplay::coordToPixel(double x, double y,
                                 int &i, int &j)
{
  j = std::round(175.5 - y/(m_hexMatrix->rowPitch()));
  i = std::round(x/(m_hexMatrix->columnPitch()) + 0.5 * (298.5 + j%2));
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
  
  // Keep axes synchronized
  connect(xAxis, SIGNAL(rangeChanged(QCPRange)),
          this, SLOT(colAxisUpdate(QCPRange)));
  connect(yAxis, SIGNAL(rangeChanged(QCPRange)),
          this, SLOT(rowAxisUpdate(QCPRange)));
  
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
  connect(m_colorScale, SIGNAL(dataRangeChanged(QCPRange)),
          this, SLOT(setDataRange(QCPRange)));
  //connect(m_colorScale, SIGNAL(gradientChanged(QCPColorGradient)),
  //        this, SLOT(setGradient(QCPColorGradient)));
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
  if (xAxis2 -> selectedParts().testFlag(QCPAxis::spAxis) ||
      xAxis2 -> selectedParts().testFlag(QCPAxis::spTickLabels))
  {
    xAxis2 -> setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
  }
  
  if (yAxis2 -> selectedParts().testFlag(QCPAxis::spAxis) ||
      yAxis2 -> selectedParts().testFlag(QCPAxis::spTickLabels))
  {
    yAxis2 -> setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
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
  else if (xAxis2->selectedParts().testFlag(QCPAxis::spAxis))
    {axisRect()->setRangeDrag(xAxis2->orientation());}
  else if (yAxis2->selectedParts().testFlag(QCPAxis::spAxis))
    {axisRect()->setRangeDrag(yAxis2->orientation());}    
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
  else if (xAxis2->selectedParts().testFlag(QCPAxis::spAxis))
    {axisRect()->setRangeZoom(xAxis2->orientation());}
  else if (yAxis2->selectedParts().testFlag(QCPAxis::spAxis))
    {axisRect()->setRangeZoom(yAxis2->orientation());}
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
