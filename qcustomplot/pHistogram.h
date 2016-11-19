/***********************************************************************
Copyright (C) 2007--2016 the X-ray Polarimetry Explorer (XPE) team.

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


#ifndef PHISTOGRAM_H
#define PHISTOGRAM_H

#include <vector>
#include <math.h>
#include <iostream>
#include <sstream>
#include <iomanip>

#include "histogramUtils.h"


class pHistogram
{
  
  /* Class describing a histogram.
     Entries and values (weights) in each bin are stored separatedly.
     Note: bin numbering starts from 0.
  */
  
  public:
    
    pHistogram(std::vector<double> binning);
    pHistogram(unsigned int nBins, double xmin, double xmax);
    
    unsigned int nbins() const {return m_nBins;}
    unsigned int entries() const;
    double sum() const;   // returns the sum of all the values stored
    double sum(double xmin, double xmax) const;
    double xMin () const {return m_xmin;}
    double xMax() const {return m_xmax;}
    double minValue() const {return m_minVal;}
    double maxValue() const {return m_maxVal;}
    std::vector<double> values() const {return m_values;}
    std::vector<double> centers() const;
    unsigned int findBin (double xValue) const;
    double binContent(unsigned int binNumber) const;
    unsigned int binEntries(unsigned int binNumber) const;
    double binWidth(unsigned int binNumber = 0) const;
    double binCenter (unsigned int binNumber) const;
    double mean() const;
    double mom2(double x0) const;
    double rms() const;
    std::pair<double, double> gaussianPeakFwhm() const;
    
    void fill(double x);
    void fill(double x, double value);
    void fillBin(unsigned int binNumber);
    void fillBin(unsigned int binNumber, double value);
    void reset();
  
  protected:
    
    pHistogram(); // declaring default constr. private, so it cannot be called
    void initialize();
    bool isBinInRange(unsigned int binNumber) const;
    
    unsigned int m_nBins;   // number of bins
    double m_xmin;    // minimum of xAxis
    double m_xmax;    // maximum of xAxis
    bool m_isLinear; // true if the binning is linear
    double m_binWidth;    // -1 if the binning is not linear
    std::vector<double> m_binEdges;   // xAxis binning
    double m_minVal;    // minimum value stored
    double m_maxVal;    // maximum value stored
    double m_sum;    // sum of all the values stored;
    std::vector<unsigned int> m_entries;    // entries in each bin
    std::vector<double> m_values;    // values in each bin
    unsigned int m_underflow;  // underflow counter
    unsigned int m_overflow;  // overflow counter
};


#endif // PHISTOGRAM_H
