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


#include "pStokesAccumulator.h"



pStokesAccumulator::pStokesAccumulator() :
  m_I(0.),
  m_Q(0.),
  m_U(0.)
{
  // Do nothing.
}


void pStokesAccumulator::reset()
{
  m_I = 0.;
  m_Q = 0.;
  m_U = 0.;
}


void pStokesAccumulator::fill(double phi)
{
  m_I += 1.;
  m_Q += cos(2.*phi);
  m_U += sin(2.*phi);
}


double pStokesAccumulator::q() const
{
  if (m_I < 0.5) {
    return 0.;
  }
  return m_Q/m_I;
}


double pStokesAccumulator::u() const
{
  if (m_I < 0.5) {
    return 0.;
  }
  return m_U/m_I;
}


std::pair<double, double> pStokesAccumulator::visibility() const
{
  double value = 0.;
  double error = 0.;
  if (m_I > 1.5) {
    value = 2*sqrt(pow(q(), 2.) + pow(u(), 2.));
    error = sqrt((2 - pow(value, 2.))/(m_I - 1.));
  }
  return std::make_pair(value, error);
}


std::pair<double, double> pStokesAccumulator::phase() const
{
  double value = 0.;
  double error = 0.;
  if (m_I > 1.5) {
    value = 0.5*atan2(u(), q());
    double visibility = 2*sqrt(pow(q(), 2.) + pow(u(), 2.));
    error = 1./(visibility*sqrt(2.*(m_I - 1.)));
  }
  return std::make_pair(value, error);
}


std::pair<double, double> pStokesAccumulator::phaseDeg() const
{
  std::pair<double, double> phi = phase();
  return std::make_pair(180.*phi.first/M_PI, 180.*phi.second/M_PI);
}


std::ostream& pStokesAccumulator::fillStream(std::ostream& os) const
{
  //os << 
  return os;
}
