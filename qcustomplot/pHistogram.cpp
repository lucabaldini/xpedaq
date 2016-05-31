#include "pHistogram.h"

pHistogram::pHistogram (unsigned int nBins, double xmin, double xmax):
                        m_nBins(nBins), m_xmin(xmin), m_xmax(xmax),
                        m_minVal(0.), m_maxVal(0.),
                        m_underflow (0), m_overflow(0)
{
  if (m_nBins < 1)
    {throw HistogramError::INVALID_BIN_NUMBER;}
  if (m_xmin > xmax)
    {throw HistogramError::INVALID_BOUNDARIES;}
  
  double width = (m_xmax - m_xmin) / nBins;
  for (unsigned int ibin = 0; ibin <= m_nBins; ++ibin)
    {m_binEdges.push_back(m_xmin + ibin * width);}
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


unsigned int pHistogram::nbins() const
{
  return m_binEdges.size() - 1;
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


double pHistogram::xMin() const
{
  return m_xmin;
}


double pHistogram::xMax() const
{
  return m_xmax;
}


double pHistogram::minValue() const
{
  return m_minVal;
}

double pHistogram::maxValue() const
{
  return m_maxVal;
}


unsigned int pHistogram::findBin (double x) const
{
  if ((x > m_xmax) || (x < m_xmin))
    {
      //std::cout << "Out of range: " << x << std::endl;
      throw HistogramError::VALUE_OUT_OF_RANGE;
    }
  unsigned int ibin = 0;
  for (ibin = 0; ibin < m_nBins; ++ibin)
  {
    if ((x >= m_binEdges.at(ibin)) && (x <= m_binEdges.at(ibin + 1)))
    {return ibin;}
  }
  return ibin; // never occurs
}


bool pHistogram::isBinInRange(unsigned int binNumber) const
{  
  if (binNumber > m_nBins)
    {return false;}
  return true;
}


double pHistogram::binContent(unsigned int binNumber) const
{
  if (! isBinInRange(binNumber))
    {throw HistogramError::BIN_OUT_OF_RANGE;}
  return m_values.at(binNumber);
}


unsigned int pHistogram::binEntries(unsigned int binNumber) const
{
  if (! isBinInRange(binNumber))
    {throw HistogramError::BIN_OUT_OF_RANGE;}
  return m_entries.at(binNumber);
}


double pHistogram::binWidth(unsigned int binNumber) const
{
  if (! isBinInRange(binNumber))
    {throw HistogramError::BIN_OUT_OF_RANGE;}
  return m_binEdges.at(binNumber+1) - m_binEdges.at(binNumber);
}


double pHistogram::binCenter(unsigned int binNumber) const
{
  if (! isBinInRange(binNumber))
    {throw HistogramError::BIN_OUT_OF_RANGE;}
  return 0.5*(m_binEdges.at(binNumber+1) + m_binEdges.at(binNumber));
}


void pHistogram::fillBin(unsigned int binNumber, double value)
{
  if (! isBinInRange(binNumber))
  {
    m_overflow += 1;
    return;
  }
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
  if (x > m_xmax)
  {
    m_overflow += 1;
    return;
  }
  else if (x < m_xmin)
  {
    m_underflow += 1;
    return;
  }
  else
    {fillBin(findBin(x), value);}
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
