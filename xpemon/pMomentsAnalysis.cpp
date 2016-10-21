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
  m_mom2long(-1.),
  m_mom2trans(-1.),
  m_mom3long(0.),
  m_statusCode(UNDEFINED)
{
  // Do-nothing.
}


int pMomentsAnalysis::run(const std::vector<event::Hit> &hits,
			  double threshold, double x0, double y0, int clusterId)
{
  // Create a set of identical weights (= 1)...
  std::vector<double> weights;
  weights.resize(hits.size());
  for (double& weight : weights) {
    weight = 1.;
  }
  // ...then run the full (weighted) moments analysis. 
  return run(hits, threshold, x0, y0, weights, clusterId);
}

int pMomentsAnalysis::run(const std::vector<event::Hit> &hits,
			  double threshold, double x0, double y0,
			  const std::vector<double> &weights, int clusterId)
{
  m_x0 = x0;
  m_y0 = y0;

  if (hits.size() < 3)
    return NOT_ENOUGH_HITS;

  // Calculate the offsets with respect to the pivot.
  std::vector<double> dx;
  std::vector<double> dy;
  std::vector<double> w;
  double wsum = 0.;
  for (unsigned int index = 0; index < hits.size(); ++index) {
    event::Hit hit = hits[index];
    if (hit.counts > threshold && hit.clusterId == clusterId){
      dx.push_back(hit.x - x0);
      dy.push_back(hit.y - y0);
      w.push_back(hit.counts * weights[index]);
      wsum += hit.counts * weights[index];
    }
  }

  // Solve for the angle of the principa axis (note that at this point
  // phi is comprised between -pi/2 and pi/2 and might indicate either
  // the major or the minor axis of the tensor of inertia).
  double A = 0;
  double B = 0;
  for (unsigned int i = 0; i < dx.size(); ++i){
    A += (dx.at(i) * dy.at(i) * w.at(i));
    B += ((pow(dy.at(i), 2.) - pow(dx.at(i), 2.)) * w.at(i));
  }
  m_phi = -0.5 * atan2(2*A, B);

  // Rotate by an angle phi and calculate the eigenvalues of the tensor
  // of inertia.
  std::vector<double> xp;
  std::vector<double> yp;
  double cphi = cos(m_phi);
  double sphi = sin(m_phi);
  for (unsigned int i = 0; i < dx.size(); ++i){
    xp.push_back(cphi * dx.at(i) + sphi * dy.at(i));
    yp.push_back(-sphi * dx.at(i) + cphi * dy.at(i));
  }
  m_mom2long = 0.;
  m_mom2trans = 0.;
  for (unsigned int i = 0; i < xp.size(); ++i){
    m_mom2long += (pow(xp.at(i), 2.) * w.at(i));
    m_mom2trans += (pow(yp.at(i), 2.) * w.at(i));
  }
  m_mom2long /= wsum;
  m_mom2trans /= wsum;
  // We want mom2long to be the highest eigenvalue, so we need to
  // check wheteher we have to swap the eigenvalues, here. Note that
  // at this point phi is still comprised between -pi/2 and pi/2.
  if (m_mom2long < m_mom2trans){
    std::swap(m_mom2long, m_mom2trans);
    m_phi -= std::copysign(0.5 * M_PI, m_phi);
  }

  // Calculate the third moment along the principal axis.
  cphi = cos(m_phi);
  sphi = sin(m_phi);
  for (unsigned int i = 0; i < dx.size(); ++i){
    xp[i] = cphi * dx.at(i) + sphi * dy.at(i);
  }
  m_mom3long = 0.;
  for (unsigned int i = 0; i < xp.size(); ++i){
    m_mom3long += (pow(xp.at(i), 3.) * w.at(i));
  }
  m_mom3long /= wsum;
 
  return SUCCESS;
}


double pMomentsAnalysis::phiFolded() const
{
  if (m_phi < -M_PI / 2.) {
    return m_phi + M_PI;
  }
  if (m_phi > M_PI / 2.) {
    return m_phi - M_PI;
  }
  return m_phi;
}


double pMomentsAnalysis::skewness() const
{
  if (m_mom2long > 0) {
    return m_mom3long/pow(m_mom2long, 1.5);
  }
  return 0.;
}


double pMomentsAnalysis::elongation() const
{
  if (m_mom2trans > 0) {
    return m_mom2long/m_mom2trans;
  }
  return -1.;
}


void pMomentsAnalysis::flip()
{
  m_phi -= std::copysign(M_PI, m_phi);
}


void pMomentsAnalysis::flip3()
{
  if (m_mom3long > 0.) {
    flip();
  }
}


void pMomentsAnalysis::draw(QCustomPlot* parentPlot, const QColor &color,
			    bool pivot, bool axis, bool ellipse,
			    int lineWidth, const Qt::PenStyle axisStyle,
			    const Qt::PenStyle ellipseStyle) const
{
  if (pivot) {
    drawPivot(parentPlot, color);
  }
  if (axis) {
    drawPrincipalAxis(parentPlot, color, lineWidth, axisStyle);
  }
  if (ellipse) {
    drawEllipse(parentPlot, color, lineWidth, ellipseStyle);
  }
}


void pMomentsAnalysis::drawPivot(QCustomPlot* parentPlot,
				 const QColor &color) const
{
  // Draw the pivot as a small circle (regular ellipse)
  QCPItemEllipse *pivotCircle = new QCPItemEllipse(parentPlot);
  QPen pivotPen = QPen();
  pivotPen.setColor(color);
  pivotCircle->setPen(pivotPen);
  QBrush pivotBrush = QBrush();
  pivotBrush.setColor(color);
  pivotBrush.setStyle(Qt::SolidPattern);
  pivotCircle->setBrush(pivotBrush);
  double radius = 0.01; // mm
  pivotCircle->topLeft->setCoords(m_x0 - radius, m_y0 + radius);
  pivotCircle->bottomRight->setCoords(m_x0 + radius, m_y0 - radius);
  /* Add the ellipse to the list of parentPlot items, so that clearItems()
     actually delete it.
     Note that we must not delete it manually, since parentPlot get the
     ownership and will delete it when necessary
  */
  parentPlot->addItem(pivotCircle);
}


void pMomentsAnalysis::drawPrincipalAxis(QCustomPlot* parentPlot,
					 const QColor &color, int lineWidth,
					 const Qt::PenStyle style) const
{
  QCPItemStraightLine *principalAxis = new  QCPItemStraightLine(parentPlot);
  QPen axisPen = QPen();
  axisPen.setStyle(style);
  axisPen.setColor(color);
  axisPen.setWidth(lineWidth);
  principalAxis->setPen(axisPen);
  // We need two points to draw the line: the pivot and another on the 
  // the direction given by the angle phi. It doesn't really matter which
  // second point one picks, since the QCPItemStraightLine is infinite.
  // I am picking a point at 0.1 mm from the pivot.  
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


void pMomentsAnalysis::drawEllipse(QCustomPlot* parentPlot, const QColor &color,
				   int lineWidth,
				   const Qt::PenStyle style) const
{
  //Draw an ellipse representing the two moments
  //Since QPainter::rotate works clockwise and our system has anticlockwise
  //angles we pass phi with a minus
  pRotableEllipse *momEllipse = new pRotableEllipse(parentPlot, -phiDeg());
  QPen ellipsePen = QPen();
  ellipsePen.setStyle(style);
  ellipsePen.setColor(color);
  ellipsePen.setWidth(lineWidth);
  momEllipse->setPen(ellipsePen);
  momEllipse->topLeft->setCoords(m_x0 - rmsLong(), m_y0 + rmsTrans());
  momEllipse->bottomRight->setCoords(m_x0 + rmsLong(), m_y0 - rmsTrans());
  /* Add the ellipse to the list of parentPlot items, so that clearItems()
     actually delete it.
     Note that we must not delete it manually, since parentPlot get the
     ownership and will delete it when necessary
  */
  parentPlot->addItem(momEllipse);
}


std::ostream& pMomentsAnalysis::fillStream(std::ostream& os) const
{
  os << "Pivot = (" << m_x0 << ", " << m_y0 << "), phi = " <<
    m_phi << ", moments = (2l: " << m_mom2long << ", 2t: " << m_mom2trans <<
    "3l: " << m_mom3long << ")";
  return os;
}
