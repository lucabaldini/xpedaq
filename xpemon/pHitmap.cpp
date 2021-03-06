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

#include "pHitmap.h"
#include "xpoldetector.h"
#include "qcustomplot.h"

pHitmap::pHitmap(const pMap* map, pColorMapOptions options) :
  pMapPlot(map, options),
  m_minPlotEdge(400) // Warning: not used at the moment
{
  //Initialize axes
  xAxis->setNumberFormat("f");
  xAxis->setNumberPrecision(0); // no decimal digits for integer index
  xAxis->setAutoTickStep(false);
  setXaxisTickStep(xAxis->range());
    
  yAxis->setNumberFormat("f");
  yAxis->setNumberPrecision(0);  // no decimal digits for integer index
  yAxis->setAutoTickStep(false);
  setYaxisTickStep(yAxis->range());  
  
  xAxis2->setRange(-7.4875, 7.4875);
  xAxis2->setLabel("x[mm]");
  xAxis2->setVisible(true);
  xAxis2->setTickLabels(true);
  
  yAxis2->setRange(-7.5991, 7.599);
  yAxis2->setLabel("y[mm]");
  yAxis2->setVisible(true);
  yAxis2->setTickLabels(true);
  
  setupAxesConnections();
  replot();
}

void pHitmap::adjustExternalMarginsForSize(int size)
{
  //Adjust external margins so that the display area is a size x size square
  //Se http://www.qcustomplot.com/documentation/classQCPAxisRect.html and
  //http://www.qcustomplot.com/documentation/thelayoutsystem.html for info
  int verticalMargin = plotLayout()->outerRect().height() - size +
                       - axisRect()->margins().top() +
                       - axisRect()->margins().bottom();
  int horizMargin = plotLayout()->outerRect().width() - size +
                    - axisRect()->margins().left() +
                    - axisRect()->margins().right() +
                    - plotLayout()->columnSpacing();
  int leftMargin = horizMargin/2;
  plotLayout()->setMargins(QMargins(leftMargin, 0, horizMargin - leftMargin,
                                    verticalMargin));
}


void pHitmap::forceSquaredAspectRatio()
{
  // Adjust external margins so that the display area is a square
  // with the maximum extension available
  //Se http://www.qcustomplot.com/documentation/classQCPAxisRect.html and
  //http://www.qcustomplot.com/documentation/thelayoutsystem.html for info
  int maxPlotWidth = plotLayout()->outerRect().width() + 
                     - axisRect()->margins().left() +
                     - axisRect()->margins().right() + 
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
  adjustExternalMarginsForSize(smallestSize);
  replot();
}


void pHitmap::offsetToPhysical(int col, int row, double &x, double &y) const
{
  x = (col - 0.5 * (298.5 + row%2 )) * (xpoldetector::kColPitch);
  y = (175.5 - row) * (xpoldetector::kRowPitch);
}


void pHitmap::phyisicalToOffset(double x, double y, int &col, int &row) const
{
  row = std::round(175.5 - y/(xpoldetector::kRowPitch));
  col = std::round(x/(xpoldetector::kColPitch) + 0.5 * (298.5 + row%2));
}


void pHitmap::xAxis2Update(QCPRange range)
{
  double xMin, xMax, y;
  offsetToPhysical(range.lower, 0., xMin, y);
  offsetToPhysical(range.upper, 0., xMax, y);
  xAxis2->setRange(xMin, xMax);
}


void pHitmap::yAxis2Update(QCPRange range)
{
  double yMin, yMax, x;
  offsetToPhysical(0., range.lower, x, yMin);
  offsetToPhysical(0., range.upper, x, yMax);
  yAxis2->setRange(yMin, yMax);
}


void pHitmap::setXaxisTickStep(QCPRange range)
{
  double interval = range.upper - range.lower;
  xAxis->setTickStep(std::max(1., interval/5.));
  replot();
}


void pHitmap::setYaxisTickStep(QCPRange range)
{
  double interval = range.upper - range.lower;
  yAxis->setTickStep(std::max(1., interval/5.));
  replot();
}


void pHitmap::setupAxesConnections()
{
  // Keep axes synchronized
  connect(xAxis, SIGNAL(rangeChanged(QCPRange)),
          this, SLOT(xAxis2Update(QCPRange)));
  connect(yAxis, SIGNAL(rangeChanged(QCPRange)),
          this, SLOT(yAxis2Update(QCPRange)));
  // Avoid duplicated tick labels
  connect(xAxis, SIGNAL(rangeChanged(QCPRange)),
          this, SLOT(setXaxisTickStep(QCPRange)));
  connect(yAxis, SIGNAL(rangeChanged(QCPRange)),
          this, SLOT(setYaxisTickStep(QCPRange)));
}


void pHitmap::paintCoordinate()
{  
  // Find offset and physical coordinates corresponding to cursor position
  // by calling the function of the inherent axis
  double xCoord = xAxis->pixelToCoord(m_cursorPos.x());
  double yCoord = yAxis->pixelToCoord(m_cursorPos.y());
  int col = static_cast<int>(xCoord+0.5); //+0.5 avoid truncating like 53.9->53
  int row = static_cast<int>(yCoord+0.5);
  double x = xAxis2->pixelToCoord(m_cursorPos.x());
  double y = yAxis2->pixelToCoord(m_cursorPos.y());
   // Find histogram cell
  int i, j;
  m_data->coordToCell(col, row, &j, &i);
  double cellContent = m_data->cell(j,i);
  // Print on the screen
  QPainter painter(this);
  painter.setPen(QPen(Qt::black));  
  //Display the info 60 pixels below the bottom-left corner
  const int pixelPicth = 60;
  QPoint textPos = axisRect()->bottomLeft();
  textPos += QPoint(0, pixelPicth);
  QString cursorText = QString("col=") + QString::number(col)
                       + QString(", row=") + QString::number(row)
                       + QString(", x=") + QString::number(x, 'f', 3)
                       + QString(", y=") + QString::number(y, 'f', 3)
                       + QString(", counts=") +QString::number(cellContent);
  painter.drawText(textPos, cursorText);  
}


void pHitmap::resizeEvent (QResizeEvent* event)
{
  pMapPlot::resizeEvent(event);
  forceSquaredAspectRatio();
}
