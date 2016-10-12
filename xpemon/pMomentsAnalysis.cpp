/***********************************************************************
Copyright (C) 2016 the X-ray Polarimetry Explorer (XPE) team.

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

#include "pMomentsAnalysis.h"


pMomentsAnalysis::pMomentsAnalysis() :
  m_x0(99.),
  m_y0(99.),  
  m_phi(9.),
  m_mom2Trans(-1.),
  m_mom2Long(-1.),
  m_skewness(0.),
  m_statusCode(UNDEFINED)
{
  // Do-nothing.
}


void pMomentsAnalysis::draw(QCustomPlot* parentPlot) const
{
  drawBarycenter(parentPlot);
  drawPrincipalAxis(parentPlot);
  drawEllipse(parentPlot);
}


void pMomentsAnalysis::drawBarycenter(QCustomPlot* parentPlot) const
{
  // Draw the barycenter as a small circle (regular ellipse)
  QCPItemEllipse *barycenterCircle = new QCPItemEllipse(parentPlot);
  QPen barycenterPen = QPen();
  barycenterPen.setColor(QColor("blue"));
  barycenterCircle->setPen(barycenterPen);
  QBrush barycenterBrush = QBrush();
  barycenterBrush.setColor(QColor("blue"));
  barycenterBrush.setStyle(Qt::SolidPattern);
  barycenterCircle->setBrush(barycenterBrush);
  double radius = 0.01; // mm
  barycenterCircle->topLeft->setCoords(m_x0 - radius, m_y0 + radius);
  barycenterCircle->bottomRight->setCoords(m_x0 + radius, m_y0 - radius);
  /* Add the ellipse to the list of parentPlot items, so that clearItems()
     actually delete it.
     Note that we must not delete it manually, since parentPlot get the
     ownership and will delete it when necessary
  */
  parentPlot->addItem(barycenterCircle);
}


void pMomentsAnalysis::drawPrincipalAxis(QCustomPlot* parentPlot) const
{
  QCPItemStraightLine *principalAxis = new  QCPItemStraightLine(parentPlot);
  QPen axisPen = QPen();
  axisPen.setStyle(Qt::DashLine);
  axisPen.setColor(QColor("blue"));
  principalAxis->setPen(axisPen);
  // We need two points to draw the line: the barycenter and another on the 
  // the direction given by the angle phi. It doesn't really matter which
  // second point one picks, since the QCPItemStraightLine is infinite.
  // I am picking a point at 0.1 mm from the barycenter.  
  double dx = 0.1; //mm
  double dy = dx * tan(m_phi); 
  principalAxis->point1->setCoords(m_x0, m_y0);
  principalAxis->point2->setCoords(m_x0 + dx, m_y0 + dy);
  /* Add the line to the list of parentPlot items, so that clearItems()
     actually delete it.
     Note that we must not delete it manually, since parentPlot get the
     ownership and will delete it when necessary
  */ 
  parentPlot->addItem(principalAxis);
}


void pMomentsAnalysis::drawEllipse(QCustomPlot* parentPlot) const
{
  //Draw an ellipse representing the two moments
  //Since QPainter::rotate works clockwise and our system has anticlockwise
  //angles we pass phi with a minus
  pRotableEllipse *momEllipse = new pRotableEllipse(parentPlot, -phiDeg());
  QPen ellipsePen = QPen();
  //ellipsePen.setStyle(Qt::DashLine);
  ellipsePen.setColor(QColor("blue"));
  momEllipse->setPen(ellipsePen);
  double longAxis = sqrt(m_mom2Long);
  double shortAxis = sqrt(m_mom2Trans);
  momEllipse->topLeft->setCoords(m_x0 - longAxis, m_y0 + shortAxis);
  momEllipse->bottomRight->setCoords(m_x0 + longAxis, m_y0 - shortAxis);
  /* Add the ellipse to the list of parentPlot items, so that clearItems()
     actually delete it.
     Note that we must not delete it manually, since parentPlot get the
     ownership and will delete it when necessary
  */
  parentPlot->addItem(momEllipse);
}
