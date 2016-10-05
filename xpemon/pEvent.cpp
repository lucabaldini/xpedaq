#include "pEvent.h"

pEvent::pEvent(int firstCol, int lastCol,
               int firstRow, int lastRow,
               const event::Adc_vec_t& adcCounts):
               pEventWindow(firstCol, lastCol, firstRow, lastRow)
{
  if (adcCounts.size() != nRows() * nColumns()) {
    std::cout << "WARNING: Buffer does not fit window size passed"
              << std::endl;
  }
  for (unsigned int i =0; i < adcCounts.size(); ++i){
    double x, y;
    pixelToCoord(pixelCoord(i), x, y);
    m_hits.push_back(event::Hit{x, y, adcCounts.at(i)});
  }
  m_cluster = pCluster();
}


adc_count_t pEvent::totalAdcCounts() const
{
  using namespace event;
  adc_count_t sum = 0; 
  for(std::vector<event::Hit>::const_iterator it = m_hits.begin();
      it != m_hits.end();
      ++it)
   {
     sum += (*it).counts;
   }
   return sum;
}


int pEvent::highestPixelAddress() const
{
  int maxPos = -1;
  int maxVal = 0;
  for (int i =0; i < evtSize(); ++i){
    if (m_hits.at(i).counts > maxVal){
      maxVal = m_hits.at(i).counts;
      maxPos = i;
    }
  }
  return maxPos;
}


const event::Hit& pEvent::highestPixel() const
{
  return m_hits.at(highestPixelAddress());
}


void pEvent::clusterize(int threshold)
{
  m_cluster.build(*this, hits(), threshold);
}


std::ostream& pEvent::fillStream(std::ostream& os) const
{
  os << "w(" << std::setfill(' ') << std::setw(3) << firstCol() << ", "
     << std::setw(3) << lastCol() << ")--(" << std::setw(3) << firstRow()
     << ", " << std::setw(3) << lastRow() << ") " << std::endl;
  return os;
}
