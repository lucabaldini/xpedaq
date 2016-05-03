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

#include "pDaqDisplay.h"

pDaqDisplay::pDaqDisplay(QWidget *parent)
  : pQtDisplayGui(parent)
{
  setTitle("Data acquisition information");
  m_stationIdLabel = addField("Station Id");
  m_runIdLabel = addField("Run Id");
  m_statusLabel = addField("Status");
  m_numDataBlocksLabel = addField("Number of data blocks acquired");
  m_numEventsLabel = addField("Number of events acquired");
  m_elapsedTimeLabel = addField("Elapsed time [s]");
  m_averageDaqEventRateLabel = addField("Average DAQ event rate [Hz]");
  m_instantDaqEventRateLabel = addField("Instant DAQ event rate [Hz]");
}
