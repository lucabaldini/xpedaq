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


#include "pModulationHistogram.h"


pModulationHistogram::pModulationHistogram(std::vector<double> binning) :
  pHistogram(binning)
{
  m_stokesAccumulator = new pStokesAccumulator();
  m_stokesAccumulator->reset();
}


pModulationHistogram::pModulationHistogram(unsigned int nBins, double xmin,
					   double xmax) :
  pHistogram(nBins, xmin, xmax)
{
  m_stokesAccumulator = new pStokesAccumulator();
}


std::pair<double, double> pModulationHistogram::visibility() const
{
  return m_stokesAccumulator->visibility();
}


std::pair<double, double> pModulationHistogram::phase() const 
{
  return m_stokesAccumulator->phase();
}


std::pair<double, double> pModulationHistogram::phaseDeg() const
{
  return m_stokesAccumulator->phaseDeg();
}


void pModulationHistogram::fill(double x)
{
  pHistogram::fill(x);
  m_stokesAccumulator->fill(x, true);
}


void pModulationHistogram::reset()
{
  pHistogram::reset();
  m_stokesAccumulator->reset();
}
