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

pEventDisplay::pEventDisplay(pColorMapOptions options) :
  m_options(options),
  m_minDisplaySurfaceSize(600),
  m_displayFirstPass(false),
  m_displaySearchRegion(false),
  m_displaySecondPass(false)
{
  //Initialize with void event
  m_event = pEvent();
  
  //Initialize axisRect
  axisRect()->setupFullAxesBox(false);
  axisRect()->setAutoMargins(QCP::msNone);
  //Leave some space at the borders
  QMargins mapMargins = QMargins(75, 50, 65, 100);
  axisRect()->setMargins(mapMargins);
  axisRect()->setMinimumSize(minAxisRectSize());
  axisRect()->center();

  //Initialize axes with default range
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
  xAxis->grid()->setVisible(false);
  yAxis->grid()->setVisible(false);
  
  //Initialize the color scale: 
  m_dataRange = QCPRange(0., 1.);
  m_colorScale = new QCPColorScale(this);
  m_colorScale->setAutoMargins(QCP::msNone);
  m_colorScaleSubLayout = new QCPLayoutGrid;
  plotLayout()->addElement(0, 1, m_colorScaleSubLayout);
  m_colorScaleSubLayout->addElement(0, 0, m_colorScale);
  m_colorScaleSubLayout->setMinimumSize(100, 0); // min. 100 pixel wide
  QMargins colorScaleMargins = QMargins(0, axisRect()->margins().top(),
                                        0, axisRect()->margins().bottom());
  m_colorScaleSubLayout->setMargins(colorScaleMargins);
  m_colorScale->axis()->setLabel(m_options.m_zTitle);
  m_colorScale->setDataRange(m_dataRange);
  m_colorScale->setGradient(m_options.m_gradientType);
  m_colorScale->axis()->setLabel(m_options.m_zTitle);
  
  //Optimize the margins to ensure the display area is a square
  setOptimalExternalMargins();

  //Initialize the matrix
  m_hexMatrix = new pHexagonMatrix(xpoldetector::kColPitch);  
  m_isSyncronized = true;

  // Initialize the search region.
  m_searchRegion = new pHorseshoe(this, "blue", 1, Qt::DashLine);
  m_searchRegion->setLayer("legend");
  setupInteractions();
}


int pEventDisplay::minAxisRectWidth() const
{
  return m_minDisplaySurfaceSize + axisRect()->margins().right() +
         axisRect()->margins().left();
}


int pEventDisplay::minAxisRectHeight() const
{
  return m_minDisplaySurfaceSize + axisRect()->margins().top()
         + axisRect()->margins().bottom();
}


QSize pEventDisplay::minAxisRectSize() const
{
  return QSize(minAxisRectWidth(), minAxisRectHeight());
}


int pEventDisplay::colorScaleLayoutWidth() const
{
  return m_colorScaleSubLayout->outerRect().width();
}


int pEventDisplay::colorScaleLayoutHeight() const
{
  return m_colorScaleSubLayout->outerRect().height();
}


QSize pEventDisplay::colorScaleLayoutSize() const
{
  return QSize(colorScaleLayoutWidth(), colorScaleLayoutHeight());
}


void pEventDisplay::adjustExternalMargins(int size)
{
  //Adjust external margins so that the display area is a square size x size
  int verticalMargin = plotLayout()->outerRect().height() - size +
                       - axisRect()->margins().top() +
                       - axisRect()->margins().bottom();
  int horizMargin = plotLayout()->outerRect().width() - size +
                    - axisRect()->margins().left() +
                    - axisRect()->margins().right() +
                    - colorScaleLayoutWidth() +
                    - plotLayout()->columnSpacing();
  int leftMargin = horizMargin/2;
  plotLayout()->setMargins(QMargins(leftMargin, 0,
                                   horizMargin - leftMargin, verticalMargin));
}


void pEventDisplay::setOptimalExternalMargins()
{
  // Adjust external margins so that the display area is a square
  // with the maximum extension available
  int maxPlotWidth = plotLayout()->outerRect().width() + 
                     - axisRect()->margins().left() +
                     - axisRect()->margins().right() + 
                     - colorScaleLayoutWidth() + 
                     - plotLayout()->columnSpacing();
  int maxPlotHeight = plotLayout()->outerRect().height() +
                      - axisRect()->margins().top() +
                      - axisRect()->margins().bottom();
  //Pick the smallest dimension (height or width)
  int smallestSize;
  if (maxPlotHeight < maxPlotWidth){
    smallestSize = maxPlotHeight;
  } else {
    smallestSize = maxPlotWidth;
  }
  adjustExternalMargins(smallestSize);
  replot();
}


void pEventDisplay::setFirstPassDisplayEnabled(int status)
{
  m_displayFirstPass = status;
  clearItems();
  draw();
}


void pEventDisplay::setSearchRegionDisplayEnabled(int status)
{
  m_displaySearchRegion = status;
  clearItems();
  draw();
}


void pEventDisplay::setSecondPassDisplayEnabled(int status)
{
  m_displaySecondPass = status;
  clearItems();
  draw();
}


void pEventDisplay::clearItems()
{
  QCustomPlot::clearItems();
  m_searchRegion->clear();
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
  for (const event::Hit& hit : m_event) {
    if (hit.counts < zmin) zmin = hit.counts;
    if (hit.counts > zmax) zmax = hit.counts;
  }
  setDataRange(QCPRange(zmin, zmax));
}


void pEventDisplay::updateAxesRange()
{
  if (m_event.isEmpty())
    return;
  double padding = 0.1; // Padding in mm.
  double xmin, xmax, ymin, ymax, x0, y0, side;
  pixelToCoord(m_event.firstCol(), m_event.firstRow(), xmin, ymax);
  pixelToCoord(m_event.lastCol(), m_event.lastRow(), xmax, ymin);
  // Add half a column pitch to the maximum coordinate, as the columns are
  // staggered.
  xmax += 0.5*m_hexMatrix->columnPitch();
  // Define the window.
  x0 = 0.5*(xmax + xmin);
  y0 = 0.5*(ymax + ymin);
  side = 0.5*std::max(xmax - xmin, ymax - ymin);
  xmin = x0 - side - padding;
  xmax = x0 + side + padding;
  ymin = y0 - side - padding;
  ymax = y0 + side + padding;
  xAxis->setRange(xmin, xmax);
  yAxis->setRange(ymin, ymax);
 
  int colMin, colMax, rowMin, rowMax;
  //Note that y is decreasing with row number, so (0,0) is left uppermost. 
  coordToPixel(xmin, ymin, colMin, rowMax);
  coordToPixel(xmax, ymax, colMax, rowMin);
  xAxis2->setRange(colMin, colMax);
  yAxis2->setRange(rowMin, rowMax); 
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
  if (m_event.isEmpty())
    return;
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
    m_hexMatrix -> hexagon(i) -> setPen(QPen(QColor(220, 220, 220)));
    // Add color only for the pixels belonging to the firts cluster.
    if (m_event.hits().at(i).clusterId == 0) {
      m_hexMatrix -> hexagon(i) -> setBrush(QBrush(color));
    }
    //else if (m_hexMatrix->border(i))
  }
}


void pEventDisplay::drawMatrix()
{
  if (m_event.isEmpty())
    return;
  double xmin, xmax, ymin, ymax;
  double padding = 0.;
  pixelToCoord(m_event.firstCol(), m_event.firstRow(), xmin, ymax);
  pixelToCoord(m_event.lastCol(), m_event.lastRow(), xmax, ymin);
  m_hexMatrix->draw(this, xmin, ymax,
                    m_event.lastCol() - m_event.firstCol() + 1,
                    m_event.lastRow() - m_event.firstRow() + 1,
                    m_event.firstCol() % 2, padding);
}


void pEventDisplay::setupSearchRegion()
{
  double x0 = m_event.moma1().x0();
  double y0 = m_event.moma1().y0();
  double rmin = 1.5*m_event.moma1().rmsLong();
  double rmax = 3.5*m_event.moma1().rmsLong();
  double phi0 = m_event.moma1().phi() - M_PI/2.;
  m_searchRegion->setup(x0, y0, rmin, rmax, phi0);
}


void pEventDisplay::draw()
{
  if (!m_isSyncronized) {
    clear();
    updateAxesRange();
    drawMatrix();
    updateMatrixColor();
  }
  clearItems();
  if (!m_event.isEmpty()) {
    if (m_displayFirstPass) {
      m_event.moma1().draw(this, "blue", true, true, true, 1, Qt::DashLine,
			   Qt::SolidLine);
    }
    if (m_displaySearchRegion) {
      setupSearchRegion();
    }
    if (m_displaySecondPass) {
      m_event.moma2().draw(this, "green", true, true, false, 1, Qt::SolidLine,
			   Qt::SolidLine);
    }
  }
  replot();
  m_isSyncronized = true;
}


void pEventDisplay::resetView()
{
  // Go back to the optimal view (all data are in axis range)
  updateDataRange();
  m_isSyncronized = false;
  draw();
}



void pEventDisplay::clear()
{
  clearPlottables(); //remove all hexagons
  clearItems(); //remove recon objects 
}


void pEventDisplay::reset()
{  
  clear();
  m_event = pEvent(); //reset the current event
  m_hexMatrix->reset(); //reset the hexagon matrix
  m_searchRegion->clear(); // Clear the search region.
  replot();
}


void pEventDisplay::pixelToCoord(int col, int row, double &x, double &y)
{
  x = (col - 0.5 * (298.5 + row%2 )) * (m_hexMatrix->columnPitch());
  y = (175.5 - row) * (m_hexMatrix->rowPitch());
}


void pEventDisplay::coordToPixel(double x, double y, int &col, int &row)
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


void pEventDisplay::mouseMoveEvent(QMouseEvent * event)
{
  m_cursorPos = event->pos();
  replot();
  QCustomPlot::mouseMoveEvent(event);
}


void pEventDisplay::paintEvent(QPaintEvent *event)
{
  QCustomPlot::paintEvent(event);
  if (!m_event.isEmpty()){
    if (m_hexMatrix->selectTest(m_cursorPos, false) > 0.){
      paintCoordinate();
    }
  }
}


void pEventDisplay::paintCoordinate()
{  
  double x = xAxis->pixelToCoord(m_cursorPos.x());
  double y = yAxis->pixelToCoord(m_cursorPos.y());
  int col, row;
  coordToPixel(x, y, col, row);
  //adc_count_t pixelContent = m_event(OffsetCoordinate(col, row)).counts;
  QPainter painter(this);
  painter.setPen(QPen(Qt::black));  
  //Display the info 80 pixels below the bottom-left corner
  const int pixelPitch = 80;
  QPoint textPos = axisRect()->bottomLeft();
  textPos += QPoint(0, pixelPitch);
  QString cursorText = QString("col=") + QString::number(col)
                       + QString(",  row=") + QString::number(row)
                       + QString(",  x=") + QString::number(x)
                       + QString(",  y=") + QString::number(y);
                       //+ QString(", counts=") + QString::number(pixelContent);
  painter.drawText(textPos, cursorText);  
}


void pEventDisplay::resizeEvent (QResizeEvent* event)
{
  QCustomPlot::resizeEvent(event);
  setOptimalExternalMargins();
}
