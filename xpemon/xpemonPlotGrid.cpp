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
  m_pulseHeightHist = new pHistogram(pulseHeightNbins, pulseHeightXmin,
                                     pulseHeightXmax);
  m_pulseHeightPlot = new pHistogramPlot(m_pulseHeightHist,
                                         pulseHeightOptions);
  m_PlotLayout -> addWidget(m_pulseHeightPlot, 0, 0);
}


void xpemonPlotGrid::setupWindowSizePlot()
{
  pBasicPlotOptions windowSizeOptions = pBasicPlotOptions("Window size",
                                             "Window size (pixel)", "n. evt");
  m_windowSizeHist = new pHistogram(windowSizeNbins, windowSizeXmin,
                                        windowSizeXmax);
  m_windowSizePlot = new pHistogramPlot(m_windowSizeHist, windowSizeOptions);
  m_PlotLayout -> addWidget(m_windowSizePlot, 0, 1);
}


void xpemonPlotGrid::setupHitMap()
{
  pColorMapOptions hitMapOptions ("Hit map", "x", "y", "counts",
                                  QCPColorGradient::gpThermal);
  m_hitMap = new pMap(xpoldetector::kNumPixelsX, -0.5, xPixelMax - 0.5,
                      xpoldetector::kNumPixelsY, -0.5, yPixelMax - 0.5);
  m_hitMapPlot = new pMapPlot(m_hitMap, hitMapOptions);
  m_PlotLayout -> addWidget(m_hitMapPlot, 1, 0);
}


void xpemonPlotGrid::setupEventDisplay()
{
  pColorMapOptions eventDisplayOptions ("Event display", "x[mm]",
                                        "y[mm]", "Adc counts",
                                        QCPColorGradient::gpThermal);
  m_eventDisplay = new pEventDisplay(eventDisplayOptions);
  m_PlotLayout->addWidget(m_eventDisplay, 1, 1);
}


void xpemonPlotGrid::updatePulseHeightPlot()
{
  m_pulseHeightPlot -> updateDisplay();
  m_pulseHeightPlot -> replot();
}


void xpemonPlotGrid::updateWindowSizePlot()
{
  m_windowSizePlot -> updateDisplay();
  m_windowSizePlot -> replot();
}


void xpemonPlotGrid::updateHitMapPlot()
{
  m_hitMapPlot -> updateDisplay();
  m_hitMapPlot -> replot();
}


void xpemonPlotGrid::updateEventDisplay(unsigned int xmin, unsigned int xmax,
                                        unsigned int ymin, unsigned int ymax,
                                     const std::vector<double>& displayValues)
{
  // See pMapPlot::setMacthingRange() for the reason of this shift
  m_eventDisplay -> setWindowRange(xmin, xmax, ymin, ymax);
  m_eventDisplay -> setAdcData(displayValues);
}


void xpemonPlotGrid::resetPlot()
{
  m_pulseHeightHist -> reset();
  m_pulseHeightPlot -> updateDisplay();
  m_windowSizeHist -> reset();
  m_windowSizePlot -> updateDisplay();
  m_hitMap -> reset();
  m_hitMapPlot -> updateDisplay();
  m_eventDisplay -> clearMap();
}

