#include "pMap.h"

pMap::pMap (unsigned int nXbins, double xmin, double xmax,
            unsigned int nYbins, double ymin, double ymax):
            m_nXbins(nXbins), m_xmin(xmin), m_xmax(xmax), m_xIsLinear(true),
            m_nYbins(nYbins), m_ymin(ymin), m_ymax(ymax), m_yIsLinear(true),
            m_minVal(0.), m_maxVal(0.)
{
  if ((m_nXbins < 1) || (m_nYbins < 1))
    {throw HistogramError::INVALID_NUMBER_OF_BINS;}
  if ((m_xmin >= m_xmax) || (m_ymin >= m_ymax))
    {throw HistogramError::INVALID_BOUNDARIES;}
  
  m_xWidth = (m_xmax - m_xmin) / m_nXbins;
  m_yWidth = (m_ymax - m_ymin) / m_nYbins;
  for (unsigned int xbin = 0; xbin <= m_nXbins; ++xbin)
    {m_xbinning.push_back(m_xmin + xbin * m_xWidth);}
  for (unsigned int ybin = 0; ybin <= m_nYbins; ++ybin)
    {m_ybinning.push_back(m_ymin + ybin * m_yWidth);}
  initialize();
}


pMap::pMap(std::vector<double> xbinning, std::vector<double> ybinning) :
           m_xbinning (xbinning), m_ybinning(ybinning),
           m_minVal(0.), m_maxVal(0.)                                                
{
  // A meaningful binning requires at least two entries
  if ((xbinning.size() < 2) || (ybinning.size() < 2))
    {throw HistogramError::INVALID_BINNING;}
  if ((!isOrdered(xbinning)) || (!isOrdered(ybinning)))
    {throw HistogramError::INVALID_BINNING;}  
  
  
  m_xIsLinear = false;
  m_xWidth = -1.;
  m_nXbins = m_xbinning.size() - 1;
  m_xmin = m_xbinning.at(0);
  m_xmax = m_xbinning.at(m_nXbins);
  
  m_yIsLinear = false;
  m_yWidth = -1.;
  m_nYbins = m_ybinning.size() - 1;
  m_ymin = m_ybinning.at(0);
  m_ymax = m_ybinning.at(m_nYbins);
  initialize();
}


void pMap::initialize()
{
  for (unsigned int ibin = 0; ibin < (m_nXbins * m_nYbins); ++ibin)
  {
    m_values.push_back(0.);
    m_entries.push_back(0);
  }
}


unsigned int pMap::entries() const
{
  unsigned int nEntries =  0.;
  for(std::vector<unsigned int>::const_iterator it = m_entries.begin();
      it != m_entries.end();
      ++it)
  {
    nEntries += (*it);
  }
  return nEntries;
}


double pMap::sum() const
{
  double sum =  0.;
  for(std::vector<double>::const_iterator it = m_values.begin();
      it != m_values.end();
      ++it)
  {
    sum+=(*it);
  }
  return sum;
}


bool pMap::areCoordsInRange(unsigned int xIndex, unsigned int yIndex) const
{  
  if (xIndex > m_nXbins) return false;
  if (yIndex > m_nYbins) return false;
  return true;
}


unsigned int pMap::index(unsigned int xIndex, unsigned int yIndex) const
{
  if (! areCoordsInRange(xIndex, yIndex))
    {throw HistogramError::BIN_OUT_OF_RANGE;}
  return xIndex + m_nXbins * yIndex;
}


double pMap::binContent(unsigned int xIndex, unsigned int yIndex) const
{
  if (! areCoordsInRange(xIndex, yIndex))
    {throw HistogramError::BIN_OUT_OF_RANGE;}
  return m_values.at(index(xIndex, yIndex));
}


unsigned int pMap::binEntries(unsigned int xIndex, unsigned int yIndex) const
{
  if (! areCoordsInRange(xIndex, yIndex))
    {throw HistogramError::BIN_OUT_OF_RANGE;}
  return m_entries.at(index(xIndex, yIndex));
}


void pMap::binCenter(unsigned int xIndex, unsigned int yIndex,
                     double &xCenter, double &yCenter) const
{
  // TODO: if binning is non linear we may want to do something different
  if (! areCoordsInRange(xIndex, yIndex))
    {throw HistogramError::BIN_OUT_OF_RANGE;}
  xCenter = 0.5 * (m_xbinning.at(xIndex) + m_xbinning.at(xIndex + 1));
  yCenter = 0.5 * (m_ybinning.at(yIndex) + m_ybinning.at(yIndex + 1));
}


double pMap::binWidthX (unsigned int xIndex, unsigned int yIndex) const
{
  if (! areCoordsInRange(xIndex, yIndex))
    {throw HistogramError::BIN_OUT_OF_RANGE;}
  if (m_xIsLinear) return m_xWidth;
  return m_xbinning.at(xIndex + 1) - m_xbinning.at(xIndex);
}


double pMap::binWidthY (unsigned int xIndex, unsigned int yIndex) const
{
  if (! areCoordsInRange(xIndex, yIndex))
    {throw HistogramError::BIN_OUT_OF_RANGE;}
  if (m_yIsLinear) return m_yWidth;
  return m_ybinning.at(yIndex + 1) - m_ybinning.at(yIndex);
}


void pMap::findBin (double x, double y,
                    unsigned int & xIndex, unsigned int & yIndex) const
{
  // TODO: fix the error logic (underflow/overflow not implemented)
  if ((x < m_xmin) || (y < m_ymin))
    {throw HistogramError::VALUE_LOWER_THAN_AXIS_RANGE;}
  if ((x > m_xmax) || (y > m_ymax))
    {throw HistogramError::VALUE_GREATER_THAN_AXIS_RANGE;}
  
  if (m_xIsLinear) xIndex = static_cast<unsigned int> ((x - m_xmin) / m_xWidth);
  else xIndex = findPosition(m_xbinning, x);
  
  if (m_yIsLinear) yIndex = static_cast<unsigned int> ((y - m_ymin) / m_yWidth);
  else yIndex = findPosition(m_ybinning, y);
}


void pMap::fillBin(unsigned int xIndex, unsigned int yIndex, double value)
{
  if (! areCoordsInRange(xIndex, yIndex))
    {throw HistogramError::BIN_OUT_OF_RANGE;}
  m_values.at(index(xIndex, yIndex)) += value;
  m_entries.at(index(xIndex, yIndex)) += 1;
  if (m_values.at(index(xIndex, yIndex)) > maxValue())
    {m_maxVal = m_values.at(index(xIndex, yIndex));}
  if (m_values.at(index(xIndex, yIndex)) < minValue())
    {m_minVal = m_values.at(index(xIndex, yIndex));} 
}


void pMap::fillBin(unsigned int xIndex, unsigned int yIndex)
{
  fillBin(xIndex, yIndex, 1.);
}


void pMap::fill(double x, double y, double value)
{
  unsigned int xIndex, yIndex;
  try
    {findBin(x, y, xIndex, yIndex);}
  catch (HistogramError histErr)
  {
    std::cout << "Out of range!" << std::endl;
    return;
  }
  fillBin(xIndex, yIndex, value);
}


void pMap::fill(double x, double y)
{
  fill(x, y, 1.);
}


void pMap::reset()
{
  for (unsigned int ibin = 0; ibin < (m_nXbins * m_nYbins); ++ibin)
  {
    m_values.at(ibin) = 0.;
    m_entries.at(ibin) = 0;
  }
  m_minVal = 0.;
  m_maxVal = 0.;
}


std::ostream& pMap::fillStream(std::ostream& os) const
{
  os << "pMap object with (" << m_nXbins << " x " << m_nYbins
     << " ) bins, range [ " << m_xmin << " , " << m_xmax << " ] - [ " 
     << m_ymin << " , " << m_ymax << " ] " << std::endl;
  return os; 
}
