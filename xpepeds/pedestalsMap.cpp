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


void PedestalsMap::fill(unsigned int pixelX, unsigned int pixelY, double value)
{
  pedestal(pixelX, pixelY).fill(value);
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
