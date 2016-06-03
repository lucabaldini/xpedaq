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
  
  /* Class describing a histogram (ROOT style).
     Entries and values (weights) in each bin are stored separatedly.
     Please note: bin numbering starts from 0 (no UNDERFLOW/OVERFLOW).
  */
  
  public:
    
    pHistogram(std::vector<double> binning);
    pHistogram(unsigned int nBins, double xmin, double xmax);
    unsigned int nbins() const;    
    unsigned int entries() const;
    double sum() const;   // sum of all the values stored
    double xMin () const;
    double xMax() const;
    double minValue() const;
    double maxValue() const;
    unsigned int findBin (double xValue) const;
    double binContent(unsigned int binNumber) const;
    unsigned int binEntries(unsigned int binNumber) const;
    double binWidth(unsigned int binNumber) const;
    double binCenter (unsigned int binNumber) const;
    void fill(double x);
    void fill(double x, double value);
    void fillBin(unsigned int binNumber);
    void fillBin(unsigned int binNumber, double value);
    void reset();
  
  private:
    
    pHistogram(); // declaring default constr. private, so it cannot be called
    void initialize();
    bool isBinInRange(unsigned int binNumber) const;
    
    unsigned int m_nBins;   // number of bins
    double m_xmin;    // minimum of xAxis
    double m_xmax;    // maximum of xAxis
    std::vector<double> m_binEdges;   // xAxis binning
    double m_minVal;    // minimum value stored
    double m_maxVal;    // maximum value stored
    std::vector<unsigned int> m_entries;    // entries in each bin
    std::vector<double> m_values;    // values in each bin
    unsigned int m_underflow;
    unsigned int m_overflow;
};


#endif // PHISTOGRAM_H
