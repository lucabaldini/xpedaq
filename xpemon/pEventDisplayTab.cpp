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

#include "pEventDisplayTab.h"


/*!
 */
pEventDisplayTab::pEventDisplayTab()
  : pQtCustomTab("Event Display")
{
  // Get as much space as possible, starting from the preferred initial size
  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

  setupEventDisplay();
}


void pEventDisplayTab::setupEventDisplay()
{
  QCPColorGradient gradient = QCPColorGradient();
  gradient.setColorInterpolation(QCPColorGradient::ciRGB);
  gradient.setColorStopAt(0, QColor(50, 0, 0));
  gradient.setColorStopAt(0.2, QColor(180, 0, 0));
  gradient.setColorStopAt(0.4, QColor(240, 30, 00));
  gradient.setColorStopAt(0.6, QColor(255, 110, 80));
  gradient.setColorStopAt(0.8, QColor(255, 180, 150));
  gradient.setColorStopAt(1, QColor(255, 255, 255));
  pColorMapOptions eventDisplayOptions ("Event display", "x[mm]",
                                        "y[mm]", "Adc counts",
                                        gradient.inverted());
  m_eventDisplay = new pEventDisplay(eventDisplayOptions);
  m_groupBoxGridLayout->addWidget(m_eventDisplay, 1, 1);
}


void pEventDisplayTab::updateEventDisplay(unsigned int xmin,
    unsigned int xmax, unsigned int ymin, unsigned int ymax,
    const std::vector<double>& displayValues)
{
  // See pMapPlot::setMacthingRange() for the reason of this shift
  m_eventDisplay -> setWindowRange(xmin, xmax, ymin, ymax);
  m_eventDisplay -> setAdcData(displayValues);
  m_eventDisplay -> draw();
}


void pEventDisplayTab::resetPlot()
{
  m_eventDisplay -> clearMap();
}

