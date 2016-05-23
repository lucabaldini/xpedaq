#include "barycenterPlot.h"

BarycenterPlot::BarycenterPlot(pColorMapOptions* options) : m_options(options)
{
  //TO DO: the options should be passed, not hardcoded
  m_options -> m_xTitle = "Pixel x";
  m_options -> m_yTitle = "Pixel y";
  m_options -> m_zTitle = "Barycenter";
  m_options -> m_gradientType = QCPColorGradient::gpPolar;
  
  axisRect()->setupFullAxesBox(true);
  setInteractions(QCP::iRangeDrag|QCP::iRangeZoom);
  this->xAxis->setLabel(m_options->m_xTitle);
  this->yAxis->setLabel(m_options->m_yTitle);
  
  m_barycenterMap = new QCPColorMap(this->xAxis,
                                    this->yAxis);
  addPlottable(m_barycenterMap);
  const unsigned int & kNx = xpoldetector::kNumPixelsX;
  const unsigned int & kNy = xpoldetector::kNumPixelsY;
  m_barycenterData = new QCPColorMapData(kNx, kNy,
                                       QCPRange(1., static_cast<double>(kNx)),
                                       QCPRange(1., static_cast<double>(kNy)));
  m_barycenterMap->setData(m_barycenterData);
  m_colorScale = new QCPColorScale(this);
  plotLayout()->addElement(0, 1, m_colorScale);
  m_colorScale->setType(QCPAxis::atRight);
  m_barycenterMap->setColorScale(m_colorScale);
  m_colorScale->axis()->setLabel(m_options->m_zTitle);
  m_barycenterMap->setGradient(m_options->m_gradientType);
  m_barycenterMap->rescaleDataRange(true);
  rescaleAxes();
  
  m_marginGroup = new QCPMarginGroup(this);
  axisRect()->setMarginGroup(QCP::msBottom|QCP::msTop, m_marginGroup);
  m_colorScale->setMarginGroup(QCP::msBottom|QCP::msTop, m_marginGroup);
  rescaleAxes();  
}


void BarycenterPlot::addPoint(double xBar, double yBar)
{
  m_barycenterData -> setData(xBar, yBar, 
                              m_barycenterData -> data(xBar, yBar) + 1.);
}



BarycenterPlot::~BarycenterPlot()
{
  //Not sure this is the right thing to do
  delete m_options;
}
