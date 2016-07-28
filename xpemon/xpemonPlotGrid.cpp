#include "xpemonPlotGrid.h"

using namespace xpemonPlotOptions;

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
  pBasicPlotOptions pulseHeightOptions = pBasicPlotOptions("Acd sum",
                              "Total over threshold evt adc counts", "n. evt");
  m_pulseHeightPlot = new pHistogramPlot(pulseHeightNbins, pulseHeightXmin,
                                         pulseHeightXmax, pulseHeightOptions);
  m_PlotLayout -> addWidget(m_pulseHeightPlot, 0, 0);
}


void xpemonPlotGrid::setupWindowSizePlot()
{
  pBasicPlotOptions windowSizeOptions = pBasicPlotOptions("Window size",
                                              "Window size (pixel)", "n. evt");
  m_windowSizePlot = new pHistogramPlot(windowSizeNbins, windowSizeXmin,
                                        windowSizeXmax, windowSizeOptions);
  m_PlotLayout -> addWidget(m_windowSizePlot, 0, 1);
}


void xpemonPlotGrid::setupHitMap()
{
  pColorMapOptions hitMapOptions ("Hit map", "x", "y", "counts",
                                  QCPColorGradient::gpThermal);
  m_hitMap = new pMapPlot(xpoldetector::kNumPixelsX, 0., xPixelMax,
                          xpoldetector::kNumPixelsY, 0., yPixelMax,
                          hitMapOptions);
  m_PlotLayout -> addWidget(m_hitMap, 1, 0);
}


void xpemonPlotGrid::setupEventDisplay()
{
  pColorMapOptions eventDisplayOptions ("Event display", "x", "y", "counts",
                                        QCPColorGradient::gpThermal);
  m_eventDisplay = new pCustomColorMapPlot(eventDisplayOptions);
  m_PlotLayout->addWidget(m_eventDisplay, 1, 1);
}


void xpemonPlotGrid::updatePulseHeightPlot(
                                  const std::vector<double>& pulseHeightValues)
{
  m_pulseHeightPlot -> updateData (pulseHeightValues);
  m_pulseHeightPlot -> replot();
}


void xpemonPlotGrid::updateWindowSizePlot(
                                   const std::vector<double>& windowSizeValues)
{
  m_windowSizePlot -> updateData (windowSizeValues);
  m_windowSizePlot -> replot();
}


void xpemonPlotGrid::updateHitMap(const std::vector<double>& hitMapValues)
{
  m_hitMap -> updateData (hitMapValues);
  m_hitMap -> replot();
}


void xpemonPlotGrid::updateEventDisplay(double xmin, double xmax,
                                        double ymin, double ymax,
                                      const std::vector<double>& displayValues)
{
  // See pMapPlot::setMacthingRange() for the reason of this shift
  m_eventDisplay -> setRange (xmin + 0.5, xmax + 0.5, ymin + 0.5, ymax + 0.5);
  // A small padding avoid unwanted truncament to the wrong unsigned int 
  m_eventDisplay -> setSize (xmax - xmin + 1.001, ymax - ymin + 1.001);
  m_eventDisplay -> updateData (displayValues);
  m_eventDisplay -> replot();
}


void xpemonPlotGrid::resetPlot()
{
  m_pulseHeightPlot -> reset();
  m_pulseHeightPlot -> replot();
  m_windowSizePlot -> reset();
  m_windowSizePlot -> replot();
  m_hitMap -> reset();
  m_hitMap -> replot();
  m_eventDisplay -> clearMap();
}

