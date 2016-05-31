#include "pEvent.h"

pEvent::pEvent(unsigned int xmin, unsigned int xmax, unsigned int ymin,
               unsigned int ymax, unsigned int bufferId,
               event::Adc_vec_t adcCounts) :
               m_xmin(xmin), m_xmax(xmax), m_ymin(ymin), m_ymax(ymax),
               m_bufferId(bufferId), m_adcCounts (adcCounts)
{
}


unsigned int pEvent::totalAdcCounts() const
{
  using namespace event;
  unsigned int sum = 0; 
  for(Adc_vec_t::const_iterator it = m_adcCounts.begin();
      it != m_adcCounts.end();
      ++it)
   {
     sum += (*it);
   }
   return sum;
}


std::ostream& pEvent::fillStream(std::ostream& os) const
{
  os << "w(" << std::setfill(' ') << std::setw(3) << xmin() << ", "
     << std::setw(3) << ymin() << ")--(" << std::setw(3) << xmax() << ", "
     << std::setw(3) << ymax() << ") "
     << std::endl;
  return os;
}
