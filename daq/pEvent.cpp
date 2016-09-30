#include "pEvent.h"

pEvent::pEvent(int firstCol, int lastCol,
               int firstRow, int lastRow,
               int bufferId, event::Adc_vec_t adcCounts): 
               m_firstCol(firstCol), m_lastCol(lastCol),
               m_firstRow(firstRow), m_lastRow(lastRow), m_bufferId(bufferId)
{
  if (adcCounts.size() != nRows() * nColumns()) {
    std::cout << "WARNING: Window size delcared in header does not match with"
                 " buffer dimension" << std::endl;
  }
  m_adcCounts = adcCounts;
}

/*** COORDINATE TRANSFORMATIONS ***/

// pixel coordinates to internal vector index
int pEvent::index(const OffsetCoordinate &p) const
{
  return p.col() + p.row() * nColumns();
}

// internal vector index to pixel coordinates
OffsetCoordinate pEvent::pixelCoord(int index) const
{
  return OffsetCoordinate(index % nColumns(), index / nColumns());
}

// cube coordinates to internal vector index
int pEvent::index(const CubeCoordinate &p) const
{
  return index(cube2Offset(p));
}

// internal vector index to cube coordinates
CubeCoordinate pEvent::cubeCoord(int index) const
{
  return offset2Cube(pixelCoord(index));
}

// pixel coordinates to physical coordinates
void pEvent::pixelToCoord(int i, int j, double &x, double &y) const
{
  x = (i - 0.5 * (298.5 + j%2 )) * (event::colPitch);
  y = (175.5 - j) * (event::rowPitch);
}

// physical coordinates to pixel coordinates
void pEvent::coordToPixel(double x, double y, int &i, int &j) const
{
  // TODO: check this transformation!
  j = std::round(175.5 - y/(event::rowPitch));
  i = std::round(x/(event::colPitch) + 0.5 * (298.5 + j%2));
}

/**********************************/


adc_count_t pEvent::pixelCounts(const OffsetCoordinate &p) const
{
  return m_adcCounts.at(index(p));
}


adc_count_t pEvent::pixelCounts(const CubeCoordinate &p) const
{
  return m_adcCounts.at(index(p));
}


adc_count_t pEvent::totalAdcCounts() const
{
  using namespace event;
  adc_count_t sum = 0; 
  for(Adc_vec_t::const_iterator it = m_adcCounts.begin();
      it != m_adcCounts.end();
      ++it)
   {
     sum += (*it);
   }
   return sum;
}


int pEvent::cubeDistance(const OffsetCoordinate &p1,
                         const OffsetCoordinate &p2) const
{
  return distance(offset2Cube(p1), offset2Cube(p2));
}


int pEvent::highestPixel() const
{
  int maxPos = -1;
  int maxVal = 0;
  for (int i =0; i < evtSize(); ++i){
    if (m_adcCounts.at(i) > maxVal){
      maxVal = m_adcCounts.at(i);
      maxPos = i;
    }
  }
  return maxPos;
}


std::ostream& pEvent::fillStream(std::ostream& os) const
{
  os << "w(" << std::setfill(' ') << std::setw(3) << firstCol() << ", "
     << std::setw(3) << lastCol() << ")--(" << std::setw(3) << firstRow()
     << ", " << std::setw(3) << lastRow() << ") " << std::endl;
  return os;
}
