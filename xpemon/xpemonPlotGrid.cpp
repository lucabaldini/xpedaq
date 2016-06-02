#include "xpemonPlotGrid.h"


xpemonPlotGrid::xpemonPlotGrid(QWidget *parent) : QWidget(parent)
{
  m_PlotLayout = new QGridLayout(this);

  // Get as much space as possible, starting from the preferred initial size
  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  
  setupPulseHeightPlot(); 
  setupWindowSizePlot();
  setupHitMap();
  setupEventDisplay();
}


void xpemonPlotGrid::setupPulseHeightPlot()
{
  unsigned int pulseHeightNbins = 100;
  double pulseHeightXmin = 0.;
  double pulseHeightXmax = 10000.;
  pBasicPlotOptions pulseHeightOptions = pBasicPlotOptions(
                                        "Evt total over threshold adc counts");
  m_pulseHeightPlot = new pHistogramPlot(pulseHeightNbins, pulseHeightXmin,
                                         pulseHeightXmax, pulseHeightOptions);
  m_PlotLayout->addWidget(m_pulseHeightPlot, 0, 0);
}


void xpemonPlotGrid::setupWindowSizePlot()
{
  unsigned int windowSizeNbins = 100;
  double windowSizeXmin = 0.;
  double windowSizeXmax = 2000.;
  pBasicPlotOptions windowSizeOptions = pBasicPlotOptions(
                                                        "Window size (pixel)");
  m_windowSizePlot = new pHistogramPlot(windowSizeNbins, windowSizeXmin,
                                        windowSizeXmax, windowSizeOptions);
  m_PlotLayout->addWidget(m_windowSizePlot, 0, 1);
}


void xpemonPlotGrid::setupHitMap()
{
  //this cast should definitely be done elsewhere
  double xmax = static_cast<double> (xpoldetector::kNumPixelsX);
  double ymax = static_cast<double> (xpoldetector::kNumPixelsY);
  pColorMapOptions hitMapOptions ("x", "y", "counts",
                                  QCPColorGradient::gpThermal);
  m_hitMap = new pMapPlot(xpoldetector::kNumPixelsX, 0., xmax,
                          xpoldetector::kNumPixelsY, 0., ymax, hitMapOptions);
  m_PlotLayout->addWidget(m_hitMap, 1, 0);
}


void xpemonPlotGrid::setupEventDisplay()
{
  //this cast should definitely be done elsewhere
  double xmax = static_cast<double> (xpoldetector::kNumPixelsX);
  double ymax = static_cast<double> (xpoldetector::kNumPixelsY);
  pColorMapOptions eventDisplayOptions ("x", "y", "counts",
                                        QCPColorGradient::gpThermal);
  m_eventDisplay = new pMapPlot(xpoldetector::kNumPixelsX, 0., xmax,
                                xpoldetector::kNumPixelsY, 0., ymax,
                                eventDisplayOptions);
  m_PlotLayout->addWidget(m_eventDisplay, 1, 1);
}


void xpemonPlotGrid::fillPulseHeight(unsigned int pHeight)
{
  m_pulseHeightPlot -> fill(static_cast<double> (pHeight));
}


void xpemonPlotGrid::writeWindow(unsigned int xmin, unsigned int xmax,
                                 unsigned int ymin, unsigned int ymax)
{
  unsigned int windowSize = (xmax -xmin + 1) * (ymax - ymin + 1);
  m_windowSizePlot -> fill(static_cast<double> (windowSize));
  m_eventDisplay -> resetData();
  m_eventDisplay -> setRange (xmin, xmax, ymin, ymax);
}


void xpemonPlotGrid::fillBarycenter(double xBar, double yBar)
{
  m_barycenterPlot -> fill(xBar,yBar);
}


void xpemonPlotGrid::writePoint(unsigned int x, unsigned int y,
                                unsigned int counts)
{
  // I am making the casting explicitly here ('fill()' requires double)
  m_hitMap -> fill(static_cast<double>(x), static_cast<double>(y),
                   static_cast<double> (counts));
  m_eventDisplay -> fill(static_cast<double>(x), static_cast<double>(y),
                         static_cast<double> (counts));
}


void xpemonPlotGrid::refreshPlot()
{
  //std::cout<<"Updating plots" << std::endl;
  m_pulseHeightPlot -> replot();
  m_windowSizePlot -> replot();
  m_hitMap->replot();
  m_eventDisplay -> replot();
}

void xpemonPlotGrid::resetPlot()
{
  m_pulseHeightPlot -> reset();
  m_windowSizePlot -> reset();
  m_hitMap -> reset();
  m_eventDisplay -> reset();
}

