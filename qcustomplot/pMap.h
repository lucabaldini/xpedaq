#ifndef PMAP_H
#define PMAP_H

#include <vector>
#include <math.h>
#include <iostream>

#include "histogramUtils.h"

class pMap
{
  
  /* Class describing a 2D histogram (ROOT style).
     Bin numbering starts from (0,0) in the lower left corner.
     Entries and values (weights) in each bin are stored separatedly.
  */
  
  public:
    
    pMap(std::vector<double> xbinning, std::vector<double> ybinning);
    pMap(unsigned int nXbins, double xmin, double xmax,
         unsigned int nYbins, double ymin, double ymax);
    unsigned int nXbins() const;
    unsigned int nYbins() const;
    unsigned int nBins() const;
    unsigned int entries() const;
    double sum() const; //sum of the values in all bins
    double xMin() const;
    double xMax() const;
    double yMin() const;
    double yMax() const;
    double minValue() const;
    double maxValue() const;   
    void findBin (double x, double y,
                  unsigned int & xIndex, unsigned int & yIndex) const;
    double binContent(unsigned int xIndex, unsigned int yIndex) const;
    unsigned int binEntries(unsigned int xIndex, unsigned int yIndex) const;
    void binCenter (unsigned int xIndex, unsigned int yIndex,
                    double &xCenter, double &yCenter) const;
    double binWidthX (unsigned int xIndex, unsigned int yIndex) const;
    double binWidthY (unsigned int xIndex, unsigned int yIndex) const;                     
    void fill(double x, double y);
    void fill(double x, double y, double value);
    void fillBin(unsigned int xIndex, unsigned int yIndex);
    void fillBin(unsigned int xIndex, unsigned int yIndex, double value);
    void reset();
   
  private:
    
    pMap(); // declaring default constr. private, so it cannot be used
    void initialize();
    void checkBinCoord (unsigned int xIndex, unsigned int yIndex) const;
    unsigned int index (unsigned int xIndex, unsigned int yIndex) const;
    
    unsigned int m_nXbins;   // number of x bins
    double m_xmin;    // minimum of xAxis
    double m_xmax;    // maximum of xAxis
    unsigned int m_nYbins;   // number of y bins
    double m_ymin;    // minimum of yAxis
    double m_ymax;    // maximum of yAxis
    std::vector<double> m_xbinning;   // xAxis binning
    std::vector<double> m_ybinning;   // xAxis binning
    double m_minVal;    // minimum value stored
    double m_maxVal;    // maximum value stored
    std::vector<unsigned int> m_entries;  // entries in each bin
    std::vector<double> m_values;    // values in each bin
};


#endif // PMAP_H
