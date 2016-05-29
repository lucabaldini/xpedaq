#include "pMap.h"

pMap::pMap (unsigned int nXbins, double xmin, double xmax,
            unsigned int nYbins, double ymin, double ymax):
            m_nXbins(nXbins), m_xmin(xmin), m_xmax(xmax),
            m_nYbins(nYbins), m_ymin(ymin), m_ymax(ymax),            
            m_minVal(0.), m_maxVal(0.)
{
  if ((m_nXbins < 1) || (m_nYbins < 1))
    {throw HistogramError::INVALID_BIN_NUMBER;}
  if ((m_xmin >= xmax) || (m_ymin >= ymax))
    {throw HistogramError::INVALID_BOUNDARIES;}
  
  double xWidth = (m_xmax - m_xmin) / m_nXbins;
  double yWidth = (m_ymax - m_ymin) / m_nYbins;
  for (unsigned int xbin = 0; xbin <= m_nXbins; ++xbin)
    {m_xbinning.push_back(m_xmin + xbin * xWidth);}
  for (unsigned int ybin = 0; ybin <= m_nYbins; ++ybin)
    {m_ybinning.push_back(m_ymin + ybin * yWidth);}
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
  
  m_nXbins = m_xbinning.size() - 1;
  m_xmin = m_xbinning.at(0);
  m_xmax = m_xbinning.at(m_nXbins);
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


unsigned int pMap::nXbins() const
{
  return m_nXbins;
}


unsigned int pMap::nYbins() const
{
  return m_nYbins;
}


unsigned int pMap::nBins() const
{
  return nXbins() * nYbins();
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


double pMap::xMin() const
{
  return m_xmin;
}


double pMap::xMax() const
{
  return m_xmax;
}


double pMap::yMin() const
{
  return m_ymin;
}


double pMap::yMax() const
{
  return m_ymax;
}


double pMap::minValue() const
{
  return m_minVal;
}

double pMap::maxValue() const
{
  return m_maxVal;
}


void pMap::checkBinCoord(unsigned int xIndex, unsigned int yIndex) const
{  
  if (xIndex > m_nXbins)
    {throw HistogramError::INVALID_BIN_NUMBER;}
  if (yIndex > m_nYbins)
    {throw HistogramError::INVALID_BIN_NUMBER;}
}


unsigned int pMap::index(unsigned int xIndex, unsigned int yIndex) const
{
  checkBinCoord(xIndex, yIndex);
  return xIndex + m_nXbins * yIndex;
}


double pMap::binContent(unsigned int xIndex, unsigned int yIndex) const
{
  checkBinCoord(xIndex, yIndex);
  return m_values.at(index(xIndex, yIndex));
}


unsigned int pMap::binEntries(unsigned int xIndex, unsigned int yIndex) const
{
  checkBinCoord(xIndex, yIndex);
  return m_entries.at(index(xIndex, yIndex));
}


void pMap::binCenter(unsigned int xIndex, unsigned int yIndex,
                     double &xCenter, double &yCenter) const
{
  //checkBinNumber(binNumber);
  //return 0.5*(m_binEdges.at(binNumber+1) + m_binEdges.at(binNumber));
}


void pMap::findBin (double x, double y,
                    unsigned int & xIndex, unsigned int & yIndex) const
{
  if ((x > m_xmax) || (x < m_xmin) || (y > m_ymax) || (y < m_ymin))
    {throw HistogramError::VALUE_OUT_OF_RANGE;}
  for (unsigned int xbin = 0; xbin < m_nXbins; ++xbin)
  {
    if ((x >= m_xbinning.at(xbin)) && (x <= m_xbinning.at(xbin + 1)))
      {xIndex = xbin;}
  }
  for (unsigned int ybin = 0; ybin < m_nYbins; ++ybin)
  {
    if ((y >= m_ybinning.at(ybin)) && (y <= m_ybinning.at(ybin + 1)))
      {yIndex = ybin;}
  }
}


void pMap::fillBin(unsigned int xIndex, unsigned int yIndex, double value)
{
  checkBinCoord(xIndex, yIndex);
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
  if ((x > m_xmax) || (x < m_xmin) || (y > m_ymax) || (y < m_ymin))
    {throw HistogramError::VALUE_OUT_OF_RANGE;}
  unsigned int xIndex, yIndex;
  findBin(x, y, xIndex, yIndex);
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
