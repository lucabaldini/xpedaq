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


#ifndef PMODULATIONHISTOGRAM_H
#define PMODULATIONHISTOGRAM_H


#include "pHistogram.h"
#include "pStokesAccumulator.h"


/*! This is a custom subclass of the basic histogram class featuring an
  additional member to keep track of the stokes parameter---so that at any
  point we know right away what the best-fit modulation visibility and angle
  are.

  It goes without saying that this only makes sense for histogramming the
  polarizations angles.
 */

class pModulationHistogram : public pHistogram
{

  public:

    pModulationHistogram(std::vector<double> binning);
    pModulationHistogram(unsigned int nBins, double xmin, double xmax);

    pStokesAccumulator *stokesAccumulator() const {return m_stokesAccumulator;}
    std::pair<double, double> visibility() const;
    std::pair<double, double> phase() const;
    std::pair<double, double> phaseDeg() const;
    void fill(double x);
    void reset();

  private:
  
    pStokesAccumulator *m_stokesAccumulator;
};

#endif // PMODULATIONHISTOGRAM_H

  
