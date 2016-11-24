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

#include "pedestalsMap.h"

using namespace pedestals;

PedestalsMap::PedestalsMap()
{
  for (unsigned int index=0; index < kNPedestal; index++)
    {m_pedMap.push_back(pRunningStat());}
}


unsigned int PedestalsMap::binIndex (unsigned int pixelX,
                                     unsigned int pixelY) const
{
  return pixelX + kNx * pixelY;
}


pRunningStat& PedestalsMap::pedestal(unsigned int pixelX, unsigned int pixelY)
{
  return m_pedMap.at(binIndex(pixelX, pixelY));
}


const pRunningStat& PedestalsMap::pedestal(unsigned int pixelX,
                                           unsigned int pixelY) const
{
  return m_pedMap.at(binIndex(pixelX, pixelY));
}


pRunningStat& PedestalsMap::operator()(unsigned int pixelX, unsigned int pixelY)
{
  return pedestal(pixelX, pixelY);
}



const pRunningStat& PedestalsMap::operator()(unsigned int pixelX, 
                                             unsigned int pixelY) const
{
  return pedestal(pixelX, pixelY);
}


int PedestalsMap::numValues (unsigned int pixelX, unsigned int pixelY) const
{
  return pedestal(pixelX, pixelY).numValues();
}


double PedestalsMap::average(unsigned int pixelX, unsigned int pixelY) const
{
  return pedestal(pixelX, pixelY).average();
}


double PedestalsMap::variance(unsigned int pixelX, unsigned int pixelY) const
{
  return pedestal(pixelX, pixelY).variance();
}


double PedestalsMap::rms(unsigned int pixelX, unsigned int pixelY) const
{
  return pedestal(pixelX, pixelY).rms();
}


double PedestalsMap::normDistance(unsigned int pixelX, unsigned int pixelY,
  double value) const
{
  
  const pRunningStat p = pedestal(pixelX, pixelY);
  double rms;
  try {
     rms = p.rms();
  }
  catch (int err) {
    return 0;
  }
  if (rms > 0.){
    return fabs((value - p.average()) / rms);
  } else {
    return 0.;
  }
}


void PedestalsMap::fill(unsigned int pixelX, unsigned int pixelY,
  double value)
{
  pedestal(pixelX, pixelY).fill(value);
}


void PedestalsMap::reset()
{
  for (unsigned int index=0; index < kNPedestal; index++)
    {m_pedMap.at(index).reset();}
}                      
