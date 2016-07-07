#include "pCustomHistogramPlot.h"

pCustomHistogramPlot::pCustomHistogramPlot(pBasicPlotOptions options) :
                                                            m_options (options)
{  
  m_bars = new QCPBars(xAxis, yAxis);
  addPlottable(m_bars);
  m_bars -> setName(m_options.m_plotName);
  xAxis -> setLabel(m_options.m_xTitle);
  yAxis -> setLabel(m_options.m_yTitle);  

  // Initializing range, binWidth and tolerance with default values
  m_centerPosTolerance = 1.e-5;
  m_bars -> keyAxis() -> setRange(0., 1.);
  m_bars -> setWidth(0.1);
   
  // Some graphical stuff
  legend -> setVisible(false);
  QFont legendFont = font();
  legendFont.setPointSize(10);
  legend -> setFont(legendFont);
  legend -> setSelectedFont(legendFont);
  setupInteractions();
}


void pCustomHistogramPlot::setTolerance(double tolerance)
{
  m_centerPosTolerance = tolerance;
}


void pCustomHistogramPlot::setKeyContent(double key, double value)
{
  /* QCPBars does not provide a method for incrementing the content of a bin.
     The default beahviour, when the function addData() is called on
     a key which already exists, is to create another entry for the same key.
     So we need to remove the old value and recreate the pair key:value with
     the updated content.
     We use removeData() on a small interval centered around the key value
     to make sure we actually remove it.
  */
  m_bars -> removeData(key - m_centerPosTolerance,
                       key + m_centerPosTolerance);
  m_bars -> addData(key, value);
  m_bars -> rescaleValueAxis();
} 


void pCustomHistogramPlot::clearBars()
{
  m_bars -> clearData();
  replot();  
}


void pCustomHistogramPlot::mouseMoveEvent(QMouseEvent * event)
{
  m_cursorPos = event -> pos();
  replot();
  QCustomPlot::mouseMoveEvent(event);
}


void pCustomHistogramPlot::paintEvent(QPaintEvent *event)
{
  QCustomPlot::paintEvent(event);
  if (m_bars -> selectTest(m_cursorPos, false) > 0.){
    paintCoordinate();
  }
}


void pCustomHistogramPlot::paintCoordinate()
{  
  double x = xAxis -> pixelToCoord(m_cursorPos.x());
  double y = yAxis -> pixelToCoord(m_cursorPos.y());
  QPainter painter(this);
  painter.setPen(QPen(Qt::black));
  painter.drawText(m_cursorPos, QString::number(x));
  // TODO: the distance between x and y should depend on the actual
  // text size, rather than being fixed
  painter.drawText(QPoint(m_cursorPos.x() + 60, m_cursorPos.y()),
                   ", ");
  painter.drawText(QPoint(m_cursorPos.x() + 70, m_cursorPos.y()),
                   QString::number(y));
}


void pCustomHistogramPlot::setupInteractions()
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
}


void pCustomHistogramPlot::selectionChanged()
{
  /* Normally, axis base line and axis tick labels are selectable separately,
     but we want the user only to be able to select the axis as a whole, so
     we tie the selected states of the tick labels and the axis base line
     together. The axis labels are made not selectable.
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


void pCustomHistogramPlot::mousePress()
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


void pCustomHistogramPlot::mouseWheel()
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
