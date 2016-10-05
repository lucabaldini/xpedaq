/***********************************************************************
Copyright (C) 2007--2016 the X-ray Polarimetry Explorer (XPE) team.

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
  setupModulationPlot();
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


void pMonitorTab::setupModulationPlot()
{
  pBasicPlotOptions modulationOptions = pBasicPlotOptions("Modulation",
                                             "theta", "n. evt");
  m_modulationHist = new pHistogram(modulationNbins, modulationThetaMin,
                                    modulationThetaMax);
  m_modulationPlot = new pHistogramPlot(m_modulationHist, modulationOptions);
  m_groupBoxGridLayout -> addWidget(m_modulationPlot, 1, 1);
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


void pMonitorTab::updateModulationPlot()
{
  m_modulationPlot -> updateDisplay();
  m_modulationPlot -> replot();
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

