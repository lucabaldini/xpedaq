#include "pRotableEllipse.h"

pRotableEllipse::pRotableEllipse(QCustomPlot* parentPlot, double angle) :
  QCPItemEllipse(parentPlot), m_angle(angle)
{
}


void pRotableEllipse::draw(QCPPainter *painter)
{
  QPointF p1 = topLeft->pixelPoint();
  QPointF p2 = bottomRight->pixelPoint();
  // Check that the points are not coincident
  if (p1.toPoint() == p2.toPoint())
    return;
  QRectF ellipseRect = QRectF(p1, p2).normalized();
  // Get the rectangle corresponding to the current visualized area,
  // plus some space for the pen size at the borders 
  QRect clip = clipRect().adjusted(-mainPen().widthF(), -mainPen().widthF(),
                                   mainPen().widthF(), mainPen().widthF());
  // Only draw if bounding rect of ellipse is visible in cliprect
  if (ellipseRect.intersects(clip))
  {
    painter->setPen(mainPen());
    painter->setBrush(mainBrush());
    
    try{ // drawEllipse sometimes throws exceptions if ellipse is too big
      painter->save();
 
      QPointF center = ellipseRect.center();
 
      painter->translate(center.x(),center.y());
      QRect newEllipseRect(-ellipseRect.width()/2, -ellipseRect.height()/2,
                           ellipseRect.width(), ellipseRect.height());

      painter->rotate(m_angle); // Note: rotate clockwise
 
      painter->drawEllipse(newEllipseRect);
 
      painter->restore();
    }
    catch (...){
      qDebug() << Q_FUNC_INFO << "Item too large for memory, "
                << "setting invisible";
      setVisible(false);
    }
  }
}
