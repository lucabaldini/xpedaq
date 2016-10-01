#include "pEvent.h"

pEvent::pEvent(int firstCol, int lastCol,
               int firstRow, int lastRow,
               const event::Adc_vec_t& adcCounts): 
               m_firstCol(firstCol), m_lastCol(lastCol),
               m_firstRow(firstRow), m_lastRow(lastRow)
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
void pEvent::pixelToCoord(const OffsetCoordinate &p,
                          double &x, double &y) const
{
  x = ((m_firstCol + p.col()) - 0.5 * (298.5 + (m_firstRow + p.row())%2 ))
      * (event::colPitch);
  y = (175.5 - (m_firstRow + p.row())) * (event::rowPitch);
}

// physical coordinates to pixel coordinates
OffsetCoordinate pEvent::coordToPixel(double x, double y) const
{
  int col, row;  
  row = std::round(175.5 - y/(event::rowPitch));
  col = std::round(x/(event::colPitch) + 0.5 * (298.5 + row%2));
  return OffsetCoordinate(col - m_firstCol, row - m_firstRow);
}

/**********************************/


adc_count_t pEvent::pixelCounts(const OffsetCoordinate &p) const
{
  return m_hits.at(index(p)).counts;
}


adc_count_t pEvent::pixelCounts(const CubeCoordinate &p) const
{
  return m_hits.at(index(p)).counts;
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


int pEvent::cubeDistance(const OffsetCoordinate &p1,
                         const OffsetCoordinate &p2) const
{
  return distance(offset2Cube(p1), offset2Cube(p2));
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


std::ostream& pEvent::fillStream(std::ostream& os) const
{
  os << "w(" << std::setfill(' ') << std::setw(3) << firstCol() << ", "
     << std::setw(3) << lastCol() << ")--(" << std::setw(3) << firstRow()
     << ", " << std::setw(3) << lastRow() << ") " << std::endl;
  return os;
}
