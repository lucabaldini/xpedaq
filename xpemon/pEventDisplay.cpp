/***********************************************************************
Copyright (C) 2007--2016 the X-ray Polarimetry Explorer (XPE) team.

For the license terms see the file LICENSE, distributed along with this
software.

This program is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation; either version 2 of the License, or (at your
option) any later version.

This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the Free Software Foundation Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
***********************************************************************/

#include "pEventDisplay.h"

pEventDisplay::pEventDisplay(pColorMapOptions options) : m_options(options)
{
  //Initialize with void event
  m_event = pEvent();
  
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
  m_hexMatrix = new pHexagonMatrix(xpoldetector::kColPitch);  
  m_isSyncronized = true;
  
  setupInteractions();
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


void pEventDisplay::loadEvent (const pEvent& evt)
{
  m_event = evt;
  m_isSyncronized = false;
  updateDataRange();  
  
}


void pEventDisplay::updateDataRange()
{
  double zmin = 0;
  double zmax = 0;
  for (const event::Hit& hit : m_event){
    if (hit.counts < zmin) zmin = hit.counts;
    if (hit.counts > zmax) zmax = hit.counts;
  }
  setDataRange(QCPRange(zmin, zmax));
}


void pEventDisplay::updateAxesRange()
{
  int maxRange = std::max(m_event.lastRow() - m_event.firstRow() + 1,
                          m_event.lastCol() - m_event.firstCol() + 1);
  int halfColExtension = (maxRange -
                         (m_event.lastCol() - m_event.firstCol() + 1)) / 2;
  int halfRowExtension = (maxRange -
                          (m_event.lastRow() - m_event.firstRow() + 1)) / 2;
  int wideColMin = m_event.firstCol() - halfColExtension - 1;
  int wideColMax = m_event.lastCol() + halfColExtension + 1;
  int wideRowMin = m_event.firstRow() - halfRowExtension - 1;
  int wideRowMax = m_event.lastRow() + halfRowExtension + 1;
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
  int nData = m_event.evtSize();
  QRgb* scanLine = new QRgb[nData];
  QCPColorGradient gradient = QCPColorGradient(m_options.m_gradientType);   
  /* This is awful, but colorize() apparently works only with double and I
     have found no other way to make this conversion */
  std::vector<double> doubleVec;
  for (const event::Hit& hit : m_event){
    doubleVec.push_back(hit.counts);
  }
  gradient.colorize (doubleVec.data(), m_dataRange, scanLine, nData); 
  for (int i=0; i<nData ;++i) {
    QColor color (scanLine[i]);
    m_hexMatrix -> hexagon(i) -> setBrush(QBrush(color));
  }
}


void pEventDisplay::drawMatrix()
{
  double xmin, xmax, ymin, ymax;
  pixelToCoord(m_event.firstCol(), m_event.firstRow(), xmin, ymax);
  pixelToCoord(m_event.lastCol(), m_event.lastRow(), xmax, ymin);
  m_hexMatrix->draw(this, xmin, ymax,
                    m_event.lastCol() - m_event.firstCol() + 1,
                    m_event.lastRow() - m_event.firstRow() + 1,
                    m_event.firstCol() % 2);
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


void pEventDisplay::pixelToCoord(int col, int row, double &x, double &y)
{
  x = (col - 0.5 * (298.5 + row%2 )) * (m_hexMatrix->columnPitch());
  y = (175.5 - row) * (m_hexMatrix->rowPitch());
}


void pEventDisplay::coordToPixel(double x, double y,
                                 int &col, int &row)
{
  row = std::round(175.5 - y/(m_hexMatrix->rowPitch()));
  col = std::round(x/(m_hexMatrix->columnPitch()) + 0.5 * (298.5 + row%2));
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
