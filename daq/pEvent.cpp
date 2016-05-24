#include "pEvent.h"

pEvent::pEvent(unsigned int xmin, unsigned int xmax, unsigned int ymin,
               unsigned int ymax, unsigned int bufferId,
               event::Adc_vec_ut adcCounts) :
               m_xmin(xmin), m_xmax(xmax), m_ymin(ymin), m_ymax(ymax),
               m_bufferId(bufferId), m_adcCounts (adcCounts)
{
}


std::vector<int> pEvent::pulseHeightsOverThreshold(int threshold)
{
  using namespace event;
  std::vector<int> heightsOT;
  for(Adc_vec_ut::iterator it = m_adcCounts.begin();
      it != m_adcCounts.end();
      ++it)
  {
    int height = static_cast<int>(*it);
    if (height > threshold){heightsOT.push_back(height);}
  }
  return heightsOT;
}


int pEvent::totPulseHeightsOverThreshold(int threshold)
{
  using namespace event;
  int sum = 0;
  for(Adc_vec_ut::iterator it = m_adcCounts.begin();
      it != m_adcCounts.end();
      ++it)
  {
    int height = static_cast<int>(*it);
    if (height > threshold){sum += height;}
  }
  return sum;
}


unsigned int pEvent::nColumns() const
{
  return (m_xmax - m_xmin + 1);
}


unsigned int pEvent::nRows() const
{
  return (m_ymax - m_ymin + 1);
}


unsigned int pEvent::totalAdcCounts() const
{
  using namespace event;
  unsigned int sum = 0; 
  for(Adc_vec_ut::const_iterator it = m_adcCounts.begin();
      it != m_adcCounts.end();
      ++it)
   {
     sum += (*it);
   }
   return sum;
}


void pEvent::barycenter (double &x, double &y)
{
  
  x = 0.;
  y = 0.;
  for (unsigned int i = 0; i < nColumns(); i ++)
  {
    for (unsigned int j = 0; j < nRows(); j ++)
    {
      x += ((m_xmin + i) * m_adcCounts [i + nColumns() * j]);
      y += ((m_ymin + j) * m_adcCounts [i + nColumns() * j]);
    }
  }
  x /= totalAdcCounts();
  y /= totalAdcCounts();
  
  /* The algorithm above is wrong. For now returning simply the center of
     the window. */
  x = 0.5*(m_xmin + m_xmax);
  y = 0.5*(m_ymin + m_ymax);
}


std::ostream& pEvent::fillStream(std::ostream& os) const
{
  os << "w(" << std::setfill(' ') << std::setw(3) << xmin() << ", "
     << std::setw(3) << ymin() << ")--(" << std::setw(3) << xmax() << ", "
     << std::setw(3) << ymax() << ") "
     << std::endl;
  return os;
}
