/***********************************************************************
Copyright (C) 2016 the X-ray Polarimetry Explorer (XPE) team.

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

#ifndef PSTOKES_ACCUMULATOR_H
#define PSTOKES_ACCUMULATOR_H

#include <iostream>
#include <cmath>


/* Class describing a "Stokes accumulator" for an event-by-event polarization
   analysis as described in https://arxiv.org/abs/1409.6214.
 */

class pStokesAccumulator
{
 public:
  
    pStokesAccumulator();

    void reset();
    void fill(double phi);

    // Getters.
    double I() const {return m_I;}
    double Q() const {return m_Q;}
    double U() const {return m_U;}

    // Normalized Stokes parameters;
    double q() const;
    double u() const;

    // Actual interesting quantities.
    std::pair<double, double> visibility() const;
    std::pair<double, double> phase() const;
    
    // Terminal formatting.
    std::ostream& fillStream(std::ostream& os) const;
    friend std::ostream& operator<<(std::ostream& os,
				    const pStokesAccumulator& accumulator)
    {return accumulator.fillStream(os);}
      
  private:

    double m_I;
    double m_Q;
    double m_U;
};

#endif //PSTOKES_ACCUMULATOR_H
