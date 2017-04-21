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

#include "pCustomColorMapPlot.h"

pCustomColorMapPlot::pCustomColorMapPlot(pColorMapOptions options) : 
  m_options(options),
  m_isLogScaleZ (false)
{
  //Initialize axes
  axisRect()->setupFullAxesBox(m_options.m_drawFullAxesBox);
  xAxis->setLabel(m_options.m_xTitle);
  yAxis->setLabel(m_options.m_yTitle);
  
  // Do not show the grid
  xAxis->grid()->setVisible(false);
  yAxis->grid()->setVisible(false);  
  
  //Intialize color map
  m_colorMap = new QCPColorMap(xAxis, yAxis);
  addPlottable(m_colorMap);
  m_colorMap->setTightBoundary(false); //display full cell at the boundaries
  m_colorMap->setInterpolate(false); //disable graphical smoothing  
  
  // Initializing the QCPColorMapData with default values
  m_data = new QCPColorMapData(1, 1, QCPRange(0., 1.), QCPRange(0., 1.)); 
  m_colorMap->setData(m_data);

  // Leave some space under the plot for displaying cursor information
  QMargins bottomMargins = QMargins(75, 10, 10, 70);
  axisRect()->setMinimumMargins(bottomMargins);

  // Initialize the color scale
  m_colorScale = new QCPColorScale(this);
  if (m_options.m_colorScale){
    plotLayout() -> addElement(0, 1, m_colorScale);
    m_colorScale->setType(QCPAxis::atRight);
    m_colorScale->axis()->setLabel(m_options.m_zTitle);
  }
  m_colorMap->setColorScale(m_colorScale);
  m_colorMap->setGradient(m_options.m_gradientType);  
  m_colorMap->rescaleDataRange(true);

  //Align thing using a margin group
  m_marginGroup = new QCPMarginGroup(this);
  axisRect() -> setMarginGroup(QCP::msBottom|QCP::msTop, m_marginGroup);
  m_colorScale -> setMarginGroup(QCP::msBottom|QCP::msTop, m_marginGroup);
  
  rescaleAxes();
  setupInteractions();
}


void pCustomColorMapPlot::setInterpolate(bool interpolate)
{
  m_colorMap->setInterpolate(interpolate);
}


void pCustomColorMapPlot::setCellContent(unsigned int xCell,
                                         unsigned int yCell,
                                         double value)
{
  m_data->setCell (xCell, yCell, value);
  m_colorMap->rescaleDataRange();
}


void pCustomColorMapPlot::setDataContent(double x, double y, double value)
{
  m_data->setData (x, y, value);
  m_colorMap->rescaleDataRange();
}


void pCustomColorMapPlot::resetView()
{
  if (m_isLogScaleZ){
    QCPRange newRange = m_data->dataBounds();
    newRange.lower = 1;
    m_colorMap->setDataRange(newRange);
  }
  else{
    m_colorMap->rescaleDataRange();
  }
  m_colorMap->rescaleAxes();
  replot();
}


void pCustomColorMapPlot::setRange (double xmin, double xmax,
                                    double ymin, double ymax)
{
  m_data->setRange(QCPRange(xmin, xmax), QCPRange(ymin, ymax));
  //Leave some margin on the axes (for better visibility) 
  double cellWidth = (xmax - xmin) / m_data->keySize();
  double cellHeight = (ymax - ymin) / m_data->valueSize();
  xAxis->setRange(xmin - 3*cellWidth, xmax + 3*cellWidth);
  yAxis->setRange(ymin - 3*cellHeight, ymax + 3*cellHeight);
}


void pCustomColorMapPlot::clearMap()
{
  m_colorMap->clearData();
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
  xAxis->setSelectableParts(QCPAxis::spAxis | QCPAxis::spTickLabels);
  yAxis->setSelectableParts(QCPAxis::spAxis | QCPAxis::spTickLabels);

  // Set the quickness of the zooming
  axisRect()->setRangeZoomFactor(0.9, 0.9);
  
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
  
  if (xAxis->selectedParts().testFlag(QCPAxis::spAxis) ||
      xAxis->selectedParts().testFlag(QCPAxis::spTickLabels))
  {
    xAxis->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
  }
  
  if (yAxis->selectedParts().testFlag(QCPAxis::spAxis) ||
      yAxis->selectedParts().testFlag(QCPAxis::spTickLabels))
  {
    yAxis->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
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
  if (!m_isLogScaleZ){
    menu->addAction("Set z to log scale", this, SLOT(setLogScaleZ()));
  }
  else {
    menu->addAction("Set z to lin scale", this, SLOT(setLinScaleZ()));
  }
  menu->addAction("Save", this, SLOT(savePlot()));
  menu->popup(mapToGlobal(pos));
}


void pCustomColorMapPlot::setLogScaleZ()
{
  m_colorMap->setDataScaleType(QCPAxis::stLogarithmic);
  m_isLogScaleZ = true;
  replot();
}


void pCustomColorMapPlot::setLinScaleZ()
{
  m_colorMap->setDataScaleType(QCPAxis::stLinear);
  m_isLogScaleZ = false;
  replot();
}


void pCustomColorMapPlot::mouseMoveEvent(QMouseEvent * event)
{
  m_cursorPos = event->pos();
  replot();
  QCustomPlot::mouseMoveEvent(event);
}


void pCustomColorMapPlot::paintEvent(QPaintEvent *event)
{
  QCustomPlot::paintEvent(event);
  if (m_colorMap->selectTest(m_cursorPos, false) > 0.){
    paintCoordinate();
  }
}


void pCustomColorMapPlot::paintCoordinate()
{  
  double x = xAxis->pixelToCoord(m_cursorPos.x());
  double y = yAxis->pixelToCoord(m_cursorPos.y());
  int i, j;
  m_data->coordToCell(x, y, &j, &i);
  double cellContent = m_data->cell(j,i);
  QPainter painter(this);
  painter.setPen(QPen(Qt::black));  
  //Display the info 60 pixels below the bottom-left corner
  QPoint textPos = axisRect()->bottomLeft();
  textPos += QPoint(0, 60);
  QString cursorText = QString("x=") + QString::number(j) + QString(", y=")
                       + QString::number(i) + QString(", bin content=")
                       + QString::number(cellContent);
  painter.drawText(textPos, cursorText);  
}


void pCustomColorMapPlot::savePlot()
{
  QString fileName;
  fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
             "", tr("Images (*.pdf)"));
  savePdf(fileName, true, 0, 0, QString("xpedaq package"),
          m_options.m_plotName);
}
