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
pMonitorTab::pMonitorTab() :
  pQtCustomTab("Monitor Plots")
{
  // Get as much space as possible, starting from the preferred initial size
  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

  setupWindowSizePlot();
  setupClusterSizePlot();
  setupPulseHeightPlot(); 
  setupModulationPlot();
}


void pMonitorTab::setupWindowSizePlot()
{
  pBasicPlotOptions windowSizeOptions = pBasicPlotOptions("Window size",
    "Window size [pixel]", "Events/bin", defaultPen, defaultBrush);
  m_windowSizeHist = new pHistogram(windowSizeNbins, windowSizeXmin,
                                    windowSizeXmax);
  m_windowSizePlot = new pHistogramPlot(m_windowSizeHist, windowSizeOptions);
  m_groupBoxGridLayout -> addWidget(m_windowSizePlot, 0, 0);
}


void pMonitorTab::setupClusterSizePlot()
{
  pBasicPlotOptions clusterSizeOptions = pBasicPlotOptions("Cluster size",
    "Cluster size [pixel]", "Events/bin", defaultPen, defaultBrush);
  m_clusterSizeHist = new pHistogram(clusterSizeNbins, clusterSizeXmin,
				     clusterSizeXmax);
  m_clusterSizePlot = new pHistogramPlot(m_clusterSizeHist, clusterSizeOptions);
  m_groupBoxGridLayout -> addWidget(m_clusterSizePlot, 0, 1);
}


void pMonitorTab::setupPulseHeightPlot()
{
  pBasicPlotOptions pulseHeightOptions = pBasicPlotOptions("ADC sum",
    "Pulse height [ADC counts]", "Events/bin", defaultPen, defaultBrush);
  m_pulseHeightHist = new pHistogram(pulseHeightNbins, pulseHeightXmin,
                                     pulseHeightXmax);
  m_pulseHeightPlot = new pHistogramPlot(m_pulseHeightHist,
                                         pulseHeightOptions);
  m_groupBoxGridLayout -> addWidget(m_pulseHeightPlot, 1, 0);  
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


void pMonitorTab::update()
{
  m_windowSizePlot -> updateDisplay();
  m_windowSizePlot -> replot();
  m_clusterSizePlot -> updateDisplay();
  m_clusterSizePlot -> replot();
  m_pulseHeightPlot -> updateDisplay();
  m_pulseHeightPlot -> replot();
  m_modulationPlot -> updateDisplay();
  m_modulationPlot -> replot();
}


void pMonitorTab::reset()
{
  m_windowSizeHist -> reset();
  m_windowSizePlot -> updateDisplay();
  m_clusterSizeHist -> reset();
  m_clusterSizePlot -> updateDisplay();
  m_pulseHeightHist -> reset();
  m_pulseHeightPlot -> updateDisplay();
  m_modulationHist -> reset();
  m_modulationPlot -> updateDisplay();
}

