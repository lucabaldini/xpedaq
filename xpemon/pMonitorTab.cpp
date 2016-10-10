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
  pBasicPlotOptions pulseHeightOptions = pBasicPlotOptions("ADC sum",
    "Pulse height [ADC counts]", "Events/bin", defaultPen, defaultBrush);
  m_pulseHeightHist = new pHistogram(pulseHeightNbins, pulseHeightXmin,
                                     pulseHeightXmax);
  m_pulseHeightPlot = new pHistogramPlot(m_pulseHeightHist,
                                         pulseHeightOptions);
  m_groupBoxGridLayout -> addWidget(m_pulseHeightPlot, 0, 0);  
}


void pMonitorTab::setupWindowSizePlot()
{
  pBasicPlotOptions windowSizeOptions = pBasicPlotOptions("Window size",
    "Window size [pixel]", "Events/bin", defaultPen, defaultBrush);
  m_windowSizeHist = new pHistogram(windowSizeNbins, windowSizeXmin,
                                    windowSizeXmax);
  m_windowSizePlot = new pHistogramPlot(m_windowSizeHist, windowSizeOptions);
  m_groupBoxGridLayout -> addWidget(m_windowSizePlot, 0, 1);
}


void pMonitorTab::setupModulationPlot()
{
  pBasicPlotOptions modulationOptions = pBasicPlotOptions("Modulation",
    "Phi [deg]", "Events/bin", defaultPen, defaultBrush);
  m_modulationHist = new pHistogram(modulationNbins, modulationThetaMin,
                                    modulationThetaMax);
  m_modulationPlot = new pHistogramPlot(m_modulationHist, modulationOptions);
  m_groupBoxGridLayout -> addWidget(m_modulationPlot, 1, 1);
}


void pMonitorTab::setupHitMap()
{
  pColorMapOptions hitMapOptions ("Hit map", "Column", "Row", "ADC counts",
                                  QCPColorGradient::gpThermal);
  /* We want the bins to be centered at their coordinate value so that,
     for example, the bins corresponding to column 0 have -0.5 < x < 0.5
  */
  unsigned int nXbins = xpoldetector::kNumPixelsX;
  unsigned int nYbins = xpoldetector::kNumPixelsY;
  double halfBinWidth = 0.5*xPixelMax/nXbins;
  double halfBinHeight = 0.5*yPixelMax/nYbins;
  m_hitMap = new pMap(nXbins, - halfBinWidth, xPixelMax - halfBinWidth,
                      nYbins, - halfBinHeight, yPixelMax - halfBinHeight);
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
  m_modulationHist -> reset();
  m_modulationPlot -> updateDisplay();
  m_hitMap -> reset();
  m_hitMapPlot -> updateDisplay();
}

