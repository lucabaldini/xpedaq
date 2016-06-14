#include "pHistogram.h"

pHistogram::pHistogram (unsigned int nBins, double xmin, double xmax):
                        m_nBins(nBins), m_xmin(xmin), m_xmax(xmax),
                        m_isLinear (true), m_minVal(0.), m_maxVal(0.),
                        m_underflow (0), m_overflow(0)
{
  if (m_nBins < 1) throw HistogramError::INVALID_NUMBER_OF_BINS;
  if (m_xmin > xmax) throw HistogramError::INVALID_BOUNDARIES;
  
  m_binWidth = (m_xmax - m_xmin) / nBins;
  for (unsigned int ibin = 0; ibin <= m_nBins; ++ibin)
    {m_binEdges.push_back(m_xmin + ibin * m_binWidth);}
  initialize();
}


pHistogram::pHistogram(std::vector<double> binning):
                        m_binEdges(binning), m_minVal(0.), m_maxVal(0.),
                        m_underflow (0), m_overflow(0)
                                                     
{
  // A meaningful binning requires at least two entries
  if (m_binEdges.size() < 2)
    {throw HistogramError::INVALID_BINNING;}
  if (!isOrdered(m_binEdges))
    {throw HistogramError::INVALID_BINNING;}
  
  m_isLinear = false;
  m_binWidth = -1.;
  m_nBins = m_binEdges.size() - 1;
  m_xmin = m_binEdges.at(0);
  m_xmax = m_binEdges.at(m_nBins);
  initialize();
}


void pHistogram::initialize()
{
  for (unsigned int ibin = 0; ibin < m_nBins; ++ibin)
  {
    m_values.push_back(0.);
    m_entries.push_back(0);
  }
}


unsigned int pHistogram::entries() const
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


double pHistogram::sum() const
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


std::vector<double> pHistogram::centers() const
{
  std::vector<double> centerVec(m_nBins);
  for (unsigned int i =0; i < m_nBins; ++i)
    {centerVec[i] = binCenter(i);}
  return centerVec;
}


bool pHistogram::isBinInRange(unsigned int binNumber) const
{  
  if (binNumber > m_nBins) return false;
  return true;
}


unsigned int pHistogram::findBin (double x) const
{
  if (x < m_xmin) throw HistogramError::VALUE_LOWER_THAN_AXIS_RANGE;
  if (x >= m_xmax) throw HistogramError::VALUE_GREATER_THAN_AXIS_RANGE;
  
  if (m_isLinear) return static_cast<unsigned int> ((x - m_xmin) / m_binWidth);
  return findPosition(m_binEdges, x);
}


double pHistogram::binContent(unsigned int binNumber) const
{
  if (! isBinInRange(binNumber)) throw HistogramError::BIN_OUT_OF_RANGE;
  return m_values.at(binNumber);
}


unsigned int pHistogram::binEntries(unsigned int binNumber) const
{
  if (! isBinInRange(binNumber)) throw HistogramError::BIN_OUT_OF_RANGE;
  return m_entries.at(binNumber);
}


double pHistogram::binWidth(unsigned int binNumber) const
{
  if (! isBinInRange(binNumber)) throw HistogramError::BIN_OUT_OF_RANGE;
  if (m_isLinear) return m_binWidth;
  return (m_binEdges.at(binNumber+1) - m_binEdges.at(binNumber));
}


double pHistogram::binCenter(unsigned int binNumber) const
{
  if (! isBinInRange(binNumber)) throw HistogramError::BIN_OUT_OF_RANGE;
  return 0.5*(m_binEdges.at(binNumber+1) + m_binEdges.at(binNumber));
}


void pHistogram::fillBin(unsigned int binNumber, double value)
{
  if (! isBinInRange(binNumber)) throw HistogramError::BIN_OUT_OF_RANGE;

  m_values.at(binNumber) += value;
  m_entries.at(binNumber) += 1;
  if (m_values.at(binNumber) > maxValue()) {m_maxVal = m_values.at(binNumber);}
  if (m_values.at(binNumber) < minValue()) {m_minVal = m_values.at(binNumber);} 
}


void pHistogram::fillBin(unsigned int binNumber)
{
  fillBin(binNumber, 1.);
}


void pHistogram::fill(double x, double value)
{
  try
  {
    unsigned int binIndex  = findBin(x);
    fillBin(binIndex, value);
  }
  catch (HistogramError histErr)
  {
   switch (histErr)
   {
     case HistogramError::VALUE_LOWER_THAN_AXIS_RANGE:
       m_underflow += 1;
       break;  
     case HistogramError::VALUE_GREATER_THAN_AXIS_RANGE:
       m_overflow += 1;
       break;
     default:
       std::cout << "Unknown error while filling value " << x << std::endl;
       break;
   }    
  }
}


void pHistogram::fill(double x)
{
  fill(x, 1.);
}


void pHistogram::reset()
{
  for (unsigned int ibin = 0; ibin < m_nBins; ++ibin)
  {
    m_values.at(ibin) = 0.;
    m_entries.at(ibin) = 0;
  }
  m_minVal = 0.;
  m_maxVal = 0.;
  m_overflow = 0;
  m_underflow = 0;
}
