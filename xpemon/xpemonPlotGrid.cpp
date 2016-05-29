#include "xpemonPlotGrid.h"

xpemonPlotGrid::xpemonPlotGrid(QWidget *parent) : QWidget(parent)
{
  m_PlotLayout = new QGridLayout(this);
  pBasicPlotOptions pulseHeightOptions = pBasicPlotOptions(
                                          "Evt total adc counts");
  unsigned int pulseHeightBins = 50;
  double pulseHeightXmin = 0.;
  double pulseHeightXmax = 5000.;
  m_pulseHeightPlot = new pHistogramPlot(pulseHeightBins, pulseHeightXmin,
                                         pulseHeightXmax, pulseHeightOptions);
  m_PlotLayout->addWidget(m_pulseHeightPlot, 0, 0);
  
  //pColorMapOptions* barycenterOpt = new pColorMapOptions();
  //m_barycenterPlot = new BarycenterPlot(barycenterOpt);
  double xmax = static_cast<double> (xpoldetector::kNumPixelsX);
  double ymax = static_cast<double> (xpoldetector::kNumPixelsY);
  m_barycenterPlot = new pMapPlot(xpoldetector::kNumPixelsX, 0., xmax,
                                  xpoldetector::kNumPixelsY, 0., ymax);
  m_PlotLayout->addWidget(m_barycenterPlot, 0, 1, 1, 1);
  
  //pColorMapOptions* hitMapOpt = new pColorMapOptions();
  //m_hitMap = new HitMap(hitMapOpt);
  m_hitMap = new pMapPlot(xpoldetector::kNumPixelsX, 0., xmax,
                          xpoldetector::kNumPixelsY, 0., ymax);
  m_PlotLayout->addWidget(m_hitMap, 1, 0, 1, 1);
  
  m_pulseHeightPlot->resize(400, 200);
  m_barycenterPlot->resize(400, 300);
  m_hitMap->resize(400, 300);
}


void xpemonPlotGrid::fillPulseHeight(int pHeight)
{
  m_pulseHeightPlot -> fill(static_cast<double> (pHeight));
}


void xpemonPlotGrid::addBarycenterPoint(double xBar, double yBar)
{
  m_barycenterPlot -> fill(xBar,yBar);
}


void xpemonPlotGrid::addHitMapPoint(double x, double y, unsigned int counts)
{
  m_hitMap -> fill(x, y, counts);
}


void xpemonPlotGrid::refreshPlot()
{
  std::cout<<"Updating plots" << std::endl;
  m_pulseHeightPlot -> replot();
  std::cout << m_pulseHeightPlot -> entries()  << std::endl;
  m_barycenterPlot->replot(); 
  m_hitMap->replot(); 
}

void xpemonPlotGrid::resetPlot()
{
  m_pulseHeightPlot->reset();
  m_barycenterPlot->reset(); 
  m_hitMap->reset(); 
}

