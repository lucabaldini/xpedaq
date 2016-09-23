#include "pMonitorTab.h"

using namespace xpemonPlotOptions;

/*!
 */
pMonitorTab::pMonitorTab(): pQtCustomTab("Monitor Plots")
{
  // Get as much space as possible, starting from the preferred initial size
  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  
  setupPulseHeightPlot(); 
  setupWindowSizePlot();
  setupHitMap();
}


void pMonitorTab::setupPulseHeightPlot()
{
  pBasicPlotOptions pulseHeightOptions = pBasicPlotOptions("Acd sum",
                             "Total over threshold evt adc counts", "n. evt");
  m_pulseHeightHist = new pHistogram(pulseHeightNbins, pulseHeightXmin,
                                     pulseHeightXmax);
  m_pulseHeightPlot = new pHistogramPlot(m_pulseHeightHist,
                                         pulseHeightOptions);
  m_groupBoxGridLayout -> addWidget(m_pulseHeightPlot, 0, 0);
}


void pMonitorTab::setupWindowSizePlot()
{
  pBasicPlotOptions windowSizeOptions = pBasicPlotOptions("Window size",
                                             "Window size (pixel)", "n. evt");
  m_windowSizeHist = new pHistogram(windowSizeNbins, windowSizeXmin,
                                        windowSizeXmax);
  m_windowSizePlot = new pHistogramPlot(m_windowSizeHist, windowSizeOptions);
  m_groupBoxGridLayout -> addWidget(m_windowSizePlot, 0, 1);
}


void pMonitorTab::setupHitMap()
{
  pColorMapOptions hitMapOptions ("Hit map", "x", "y", "counts",
                                  QCPColorGradient::gpThermal);
  m_hitMap = new pMap(xpoldetector::kNumPixelsX, -0.5, xPixelMax - 0.5,
                      xpoldetector::kNumPixelsY, -0.5, yPixelMax - 0.5);
  m_hitMapPlot = new pMapPlot(m_hitMap, hitMapOptions);
  m_groupBoxGridLayout -> addWidget(m_hitMapPlot, 1, 0);
}


void pMonitorTab::updatePulseHeightPlot()
{
  m_pulseHeightPlot -> updateDisplay();
  m_pulseHeightPlot -> replot();
}


void pMonitorTab::updateWindowSizePlot()
{
  m_windowSizePlot -> updateDisplay();
  m_windowSizePlot -> replot();
}


void pMonitorTab::updateHitMapPlot()
{
  m_hitMapPlot -> updateDisplay();
  m_hitMapPlot -> replot();
}


void pMonitorTab::resetPlot()
{
  m_pulseHeightHist -> reset();
  m_pulseHeightPlot -> updateDisplay();
  m_windowSizeHist -> reset();
  m_windowSizePlot -> updateDisplay();
  m_hitMap -> reset();
  m_hitMapPlot -> updateDisplay();
}

