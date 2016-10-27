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


#include "pHitmapTab.h"


using namespace xpemonPlotOptions;


pHitmapTab::pHitmapTab() :
  pQtCustomTab("Hitmap")
{
  setup();
}


void pHitmapTab::update()
{
  m_hitmapPlot -> updateDisplay();
  m_hitmapPlot -> replot();
}


void pHitmapTab::reset()
{
  m_hitmap -> reset();
  m_hitmapPlot -> updateDisplay();
}

  
void pHitmapTab::setup()
{
  pColorMapOptions hitmapOptions ("Hit map", "Column", "Row", "ADC counts",
                                  QCPColorGradient::gpThermal);
  /* We want the bins to be centered at their coordinate value so that,
     for example, the bins corresponding to column 0 have -0.5 < x < 0.5
  */
  unsigned int nXbins = xpoldetector::kNumPixelsX;
  unsigned int nYbins = xpoldetector::kNumPixelsY;
  double halfBinWidth = 0.5*xPixelMax/nXbins;
  double halfBinHeight = 0.5*yPixelMax/nYbins;
  m_hitmap = new pMap(nXbins, - halfBinWidth, xPixelMax - halfBinWidth,
                      nYbins, - halfBinHeight, yPixelMax - halfBinHeight);
  m_hitmapPlot = new pMapPlot(m_hitmap, hitmapOptions);
  m_hitmapPlot->setInterpolate(true);
  m_hitmapPlot->axisRect()->setMinimumSize(740, 700);
  m_hitmapPlot->axisRect()->setMaximumSize(740, 700);
  m_groupBoxGridLayout->addWidget(m_hitmapPlot, 1, 0);
}


