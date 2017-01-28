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


/*!
 */
pMonitorTab::pMonitorTab(bool showModulationPlot) :
  pQtCustomTab("Monitor Plots"),
  m_showModulationPlot(showModulationPlot)
{
  // Get as much space as possible, starting from the preferred initial size
  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  setupWindowSizePlot();
  setupPulseHeightPlot();
  setupModulationPlot();
  setupClusterSizePlot();
  setupHitmapPlot();
}


void pMonitorTab::setupWindowSizePlot()
{
  using namespace xpemonPlotOptions;
  pBasicPlotOptions windowSizeOptions = pBasicPlotOptions("Window size",
    "Window size [pixel]", "Events/bin", defaultPen, defaultBrush);
  m_windowSizeHist = new pHistogram(windowSizeNbins, windowSizeXmin,
                                    windowSizeXmax);
  m_windowSizePlot = new pHistogramPlot(m_windowSizeHist, windowSizeOptions);
  m_windowSizePlot->axisRect()->setAutoMargins(QCP::msNone);
  m_windowSizePlot->axisRect()->setMargins(defaultMargins);
  m_groupBoxGridLayout->addWidget(m_windowSizePlot, 0, 0);
}


void pMonitorTab::setupClusterSizePlot()
{
  using namespace xpemonPlotOptions;
  pBasicPlotOptions clusterSizeOptions = pBasicPlotOptions("Cluster size",
    "Cluster size [pixel]", "Events/bin", defaultPen, defaultBrush);
  m_clusterSizeHist = new pHistogram(clusterSizeNbins, clusterSizeXmin,
				     clusterSizeXmax);
  m_clusterSizePlot = new pHistogramPlot(m_clusterSizeHist, clusterSizeOptions);
  if (!m_showModulationPlot) {
    m_groupBoxGridLayout->addWidget(m_clusterSizePlot, 1, 1);
  }
}


void pMonitorTab::setupPulseHeightPlot()
{
  using namespace xpemonPlotOptions;
  pBasicPlotOptions pulseHeightOptions = pBasicPlotOptions("ADC sum",
    "Pulse height [ADC counts]", "Events/bin", defaultPen, defaultBrush);
  m_pulseHeightHist = new pHistogram(pulseHeightNbins, pulseHeightXmin,
                                     pulseHeightXmax);
  m_pulseHeightPlot = new pHistogramPlot(m_pulseHeightHist,
                                         pulseHeightOptions);
  m_pulseHeightPlot->axisRect()->setAutoMargins(QCP::msNone);
  m_pulseHeightPlot->axisRect()->setMargins(defaultMargins);
  m_groupBoxGridLayout->addWidget(m_pulseHeightPlot, 0, 1);
  // Add a graph for the gaussian fit.
  m_pulseHeightPlot->addGraph();
  m_pulseHeightPlot->graph(0)->setPen(QPen(Qt::red));
  m_pulseHeightStatBox = new pStatBox(m_pulseHeightPlot, 0.05, 0.0);
  m_pulseHeightStatBox->addField("Peak", 0);
  m_pulseHeightStatBox->addField("FWHM", 2);
  m_pulseHeightStatBox->addField("Tail", 3);
  resetPulseHeightFit();
}


void pMonitorTab::setupModulationPlot()
{
  using namespace xpemonPlotOptions;
  pBasicPlotOptions modulationOptions = pBasicPlotOptions("Modulation",
    "Phi [deg]", "Events/bin", defaultPen, defaultBrush);
  m_modulationHist = new pModulationHistogram(modulationNbins,
					      modulationThetaMin,
					      modulationThetaMax);
  m_modulationPlot = new pHistogramPlot(m_modulationHist, modulationOptions);
  m_modulationPlot->axisRect()->setAutoMargins(QCP::msNone);
  m_modulationPlot->axisRect()->setMargins(defaultMargins);
  if (m_showModulationPlot) {
    m_groupBoxGridLayout->addWidget(m_modulationPlot, 1, 1);
    // Add a graph for the cosine square fit.
    m_modulationPlot->addGraph();
    m_modulationPlot->graph(0)->setPen(QPen(Qt::red));
    m_modulationStatBox = new pStatBox(m_modulationPlot, 0.05, 0.0);
    m_modulationStatBox->addField("Modulation", 1);
    m_modulationStatBox->addField("Phase", 1);
    resetModulationFit();
  }
}


/*!
 */
void pMonitorTab::updateModulationFit()
{
  if (m_modulationHist->sum() > 10) {
    std::pair<double, double> visibility = m_modulationHist->visibility();
    std::pair<double, double> phase = m_modulationHist->phaseDeg();
    const int numPoints = 100;
    double norm = m_modulationHist->sum()/(double)m_modulationHist->nbins();
    QVector<double> x(numPoints), y(numPoints);
    for (int i = 0; i < numPoints; ++i) {
      x[i] = -180. + i/float(numPoints)*360;
      double cosPhi = cos(x[i]/180*3.1415 - phase.first/180*3.1415);
      y[i] = norm*((1 - visibility.first) + 2*visibility.first*pow(cosPhi, 2.));
    }
    m_modulationPlot->graph(0)->setData(x, y);
    m_modulationStatBox->setField("Modulation", 100*visibility.first,
				  100*visibility.second, "%");
    m_modulationStatBox->setField("Phase", phase.first, phase.second, "deg");
  }
}


/*!
 */
void pMonitorTab::resetModulationFit()
{
  m_modulationStatBox->reset();
  QVector<double> x(0), y(0);
  m_modulationPlot->graph(0)->setData(x, y);
}


/*!
 */
void pMonitorTab::updatePulseHeightFit()
{
  if (m_pulseHeightHist->sum() > 10) {
    std::pair<double, double> params = m_pulseHeightHist->gaussianPeakFwhm();
    double peak = params.first;
    double rms = params.second/2.355;
    double xmin = peak - 2.5*rms;
    double xmax = peak + 2.5*rms;
    double tail = m_pulseHeightHist->sum(0., xmin)/m_pulseHeightHist->sum();
    m_pulseHeightStatBox->setField("Peak", peak);
    m_pulseHeightStatBox->setField("FWHM", 100.*params.second/peak);
    m_pulseHeightStatBox->setField("Tail", tail);
    const int numPoints = 100;
    double norm = 0.3989422804014327/rms*m_pulseHeightHist->sum(xmin, xmax)*
      m_pulseHeightHist->binWidth();
    QVector<double> x(numPoints), y(numPoints);
    for (int i = 0; i < numPoints; ++i) {
      x[i] = m_pulseHeightHist->xMin() +
	i/float(numPoints)*m_pulseHeightHist->xMax();
      y[i] = norm*exp(-0.5*pow((x[i] - peak), 2.)/pow(rms, 2.));
    }
    m_pulseHeightPlot->graph(0)->setData(x, y);
  }
}


/*!
 */
void pMonitorTab::resetPulseHeightFit()
{
  m_pulseHeightStatBox->reset();
  QVector<double> x(0), y(0);
  m_pulseHeightPlot->graph(0)->setData(x, y);
}


void pMonitorTab::setupHitmapPlot()
{
  using namespace xpemonPlotOptions;
  pColorMapOptions hitmapOptions ("Hit map", "Column", "Row", "ADC counts",
                                  QCPColorGradient::gpThermal, false, false);
  /* We want the bins to be centered at their coordinate value so that,
     for example, the bins corresponding to column 0 have -0.5 < x < 0.5
  */
  unsigned int nXbins = xpoldetector::kNumPixelsX;
  unsigned int nYbins = xpoldetector::kNumPixelsY;
  double halfBinWidth = 0.5*xPixelMax/nXbins;
  double halfBinHeight = 0.5*yPixelMax/nYbins;
  m_hitmap = new pMap(nXbins, - halfBinWidth, xPixelMax - halfBinWidth,
                      nYbins, - halfBinHeight, yPixelMax - halfBinHeight);
  m_hitmapPlot = new pHitmap(m_hitmap, hitmapOptions);
  //Revert y axis so that it matches the XPOL coordinate system
  m_hitmapPlot->yAxis->setRangeReversed(true);
  //Apply a smoothing to the events
  //m_hitmapPlot->setInterpolate(true);
  m_hitmapPlot->axisRect()->setMinimumSize(205, 240);
  //m_hitmapPlot->axisRect()->setMaximumSize(705, 740);
  m_groupBoxGridLayout->addWidget(m_hitmapPlot, 1, 0);
}


void pMonitorTab::update()
{
  m_windowSizePlot->updateDisplay();
  m_windowSizePlot->replot();
  updatePulseHeightFit();
  m_pulseHeightPlot->updateDisplay();
  m_pulseHeightPlot->replot();
  if (m_showModulationPlot) {
    updateModulationFit();
    m_modulationPlot->updateDisplay();
    m_modulationPlot->replot();
  } else {
    m_clusterSizePlot->updateDisplay();
    m_clusterSizePlot->replot();
  }
  m_hitmapPlot->updateDisplay();
  m_hitmapPlot->replot();
}


void pMonitorTab::reset()
{
  m_windowSizeHist->reset();
  m_windowSizePlot->updateDisplay();
  resetPulseHeightFit();
  m_pulseHeightHist->reset();
  m_pulseHeightPlot->updateDisplay();
  if (m_showModulationPlot) {
    resetModulationFit();
    m_modulationHist->reset();
    m_modulationPlot->updateDisplay();
  } else {
    m_clusterSizeHist->reset();
    m_clusterSizePlot->updateDisplay();
  }
  m_hitmap->reset();
  m_hitmapPlot->updateDisplay();
}

