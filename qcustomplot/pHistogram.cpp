/***********************************************************************
Copyright (C) 2007--2017 the X-ray Polarimetry Explorer (XPE) team.

For the license terms see the file LICENSE, distributed along with this
software.

This program is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation; either version 2 of the License, or (at your
option) any later version.

This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the Free Software Foundation Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
***********************************************************************/


#include "pHistogram.h"


pHistogram::pHistogram (unsigned int nBins, double xmin, double xmax) :
  m_nBins(nBins),
  m_xmin(xmin),
  m_xmax(xmax),
  m_isLinear(true),
  m_minVal(0.),
  m_maxVal(0.),
  m_sum (0),
  m_underflow (0),
  m_overflow(0)
{
  if (m_nBins < 1) throw HistogramError::INVALID_NUMBER_OF_BINS;
  if (m_xmin > xmax) throw HistogramError::INVALID_BOUNDARIES;
  
  m_binWidth = (m_xmax - m_xmin) / nBins;
  for (unsigned int bin = 0; bin <= m_nBins; ++bin) {
    m_binEdges.push_back(m_xmin + bin * m_binWidth);
  }
  initialize();
}


pHistogram::pHistogram(std::vector<double> binning) :
  m_binEdges(binning),
  m_minVal(0.),
  m_maxVal(0.),
  m_underflow (0),
  m_overflow(0)                                                   
{
  // A meaningful binning requires at least two entries
  if (m_binEdges.size() < 2) {
    throw HistogramError::INVALID_BINNING;
  }
  if (!isOrdered(m_binEdges)) {
    throw HistogramError::INVALID_BINNING;
  }
  m_isLinear = false;
  m_binWidth = -1.;
  m_nBins = m_binEdges.size() - 1;
  m_xmin = m_binEdges.at(0);
  m_xmax = m_binEdges.at(m_nBins);
  initialize();
}


void pHistogram::initialize()
{
  for (unsigned int bin = 0; bin < m_nBins; ++bin) {
    m_values.push_back(0.);
    m_entries.push_back(0);
  }
}


unsigned int pHistogram::entries() const
{
  unsigned int nEntries =  0.;
  for (const auto &it : m_entries) {
    nEntries += it;
  }
  return nEntries;
}


double pHistogram::sum() const
{
  return m_sum;
}


double pHistogram::sum(double xmin, double xmax) const
{
  double val = 0.;
  for (unsigned int bin = 0; bin < m_nBins; ++bin) {
    double x = binCenter(bin);
    if (x >= xmin && x <= xmax) {
      val += binContent(bin);
    }
  }
  return val;
}


std::vector<double> pHistogram::centers() const
{
  std::vector<double> centerVec(m_nBins);
  for (unsigned int i = 0; i < m_nBins; ++i) {
    centerVec[i] = binCenter(i);
  }
  return centerVec;
}


bool pHistogram::isBinInRange(unsigned int binNumber) const
{  
  if (binNumber > m_nBins) {
    return false;
  }
  return true;
}


unsigned int pHistogram::findBin (double x) const
{
  if (x < m_xmin) {
    throw HistogramError::VALUE_LOWER_THAN_AXIS_RANGE;
  }
  if (x >= m_xmax) {
    throw HistogramError::VALUE_GREATER_THAN_AXIS_RANGE;
  } 
  if (m_isLinear) {
    return static_cast<unsigned int> ((x - m_xmin) / m_binWidth);
  }
  return findPosition(m_binEdges, x);
}


double pHistogram::binContent(unsigned int binNumber) const
{
  if (! isBinInRange(binNumber)) {
    throw HistogramError::BIN_OUT_OF_RANGE;
  }
  return m_values.at(binNumber);
}


unsigned int pHistogram::binEntries(unsigned int binNumber) const
{
  if (! isBinInRange(binNumber)) {
    throw HistogramError::BIN_OUT_OF_RANGE;
  }
  return m_entries.at(binNumber);
}


double pHistogram::binWidth(unsigned int binNumber) const
{
  if (! isBinInRange(binNumber)) {
    throw HistogramError::BIN_OUT_OF_RANGE;
  }
  if (m_isLinear) {
    return m_binWidth;
  }
  return (m_binEdges.at(binNumber+1) - m_binEdges.at(binNumber));
}


double pHistogram::binCenter(unsigned int binNumber) const
{
  if (! isBinInRange(binNumber)) {
    throw HistogramError::BIN_OUT_OF_RANGE;
  }
  return 0.5*(m_binEdges.at(binNumber+1) + m_binEdges.at(binNumber));
}


double pHistogram::binLowEdge(unsigned int binNumber) const
{
  if (!isBinInRange(binNumber)) {
    throw HistogramError::BIN_OUT_OF_RANGE;
  }
  return m_binEdges.at(binNumber);
}


double pHistogram::binHighEdge(unsigned int binNumber) const
{
  if (!isBinInRange(binNumber)) {
    throw HistogramError::BIN_OUT_OF_RANGE;
  }
  return m_binEdges.at(binNumber + 1);
}


/*! Return the quantile point corresponding to a given value of the histogram
cumulative function.

This is simply interpolating linearly over the bin edges.
*/
double pHistogram::quantile(double q) const
{
  double sum = 0.;
  for (unsigned int bin = 0; bin < m_nBins; ++bin) {
    sum += binContent(bin)/m_sum;
    if (sum >= q) {
      if (bin == 0) {
	return binCenter(bin);
      } else {
	double x1 = binLowEdge(bin);
	double x2 = binHighEdge(bin);
	double y1 = sum - binContent(bin)/m_sum;
	double y2 = sum;
	return x1 + (x2 - x1)*(q - y1)/(y2 - y1);
      }
      return binCenter(bin);
    }
  }
}


/*! Return the mean value of the histogram.
  
  We might have done this in an unbinned fashion using a pRunningStat object,
  but since the histogram class is setup to handle weights, we should do the
  same in the running stat class.
*/
double pHistogram::mean() const
{
  double mean = 0.;
  if (m_sum > 0) {
    for (unsigned int bin = 0; bin < m_nBins; ++bin) {
      mean += binCenter(bin) * binContent(bin);
    }
    mean /= m_sum;
  }
  return mean;
}


/*! Return the second moment around a generic value x0. (And yes, I am not
  trying to be clever here---this is actually pretty dumb.)
  
  We provide this convenience function that can be called to calculate the
  variance so that we can take advantage of it when we need the mean and the
  variance---in that case we can cache the mean and don't have to calculate
  it twice.
 */
double pHistogram::mom2(double x0) const
{
  double mom2 = 0.;
  if (m_sum > 0) {
    for (unsigned int bin = 0; bin < m_nBins; ++bin) {
      mom2 += pow((binCenter(bin) - x0), 2.) * binContent(bin);
    }
    mom2 /= m_sum;
  }
  return mom2;
}


/*! Return the rms of the histogram.
 */
double pHistogram::rms() const
{
  return sqrt(mom2(mean()));
}


/*! This could be streamlined if we had a function calculating an array of
quantiles, so that we can loop on the bins exactly once.
 */
std::pair<double, double> pHistogram::gaussianPeakFwhm() const
{
  double peak = quantile(0.5);
  double fwhm = 2*(quantile(0.8805) - peak);
  return std::make_pair(peak, fwhm);
}


void pHistogram::fillBin(unsigned int binNumber, double value)
{
  if (! isBinInRange(binNumber)) {
    throw HistogramError::BIN_OUT_OF_RANGE;
  }
  m_values.at(binNumber) += value;
  m_entries.at(binNumber) += 1;
  m_sum += value;
  if (m_values.at(binNumber) > maxValue()) {
    m_maxVal = m_values.at(binNumber);
  }
  if (m_values.at(binNumber) < minValue()) {
    m_minVal = m_values.at(binNumber);
  } 
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
  for (unsigned int bin = 0; bin < m_nBins; ++bin) {
    m_values.at(bin) = 0.;
    m_entries.at(bin) = 0;
  }
  m_minVal = 0.;
  m_maxVal = 0.;
  m_sum = 0.;
  m_overflow = 0;
  m_underflow = 0;
}
