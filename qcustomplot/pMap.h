#ifndef PMAP_H
#define PMAP_H

#include <vector>
#include <math.h>
#include <iostream>
#include <sstream>
#include <iomanip>

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
    
    unsigned int nXbins() const {return m_nXbins;}
    unsigned int nYbins() const {return m_nYbins;}
    unsigned int nBins() const {return nXbins() * nYbins();}
    unsigned int entries() const;
    double sum() const; //sum of the values in all bins
    double xMin() const {return m_xmin;}
    double xMax() const {return m_xmax;}
    double yMin() const {return m_ymin;}
    double yMax() const {return m_ymax;}
    double minValue() const {return m_minVal;}
    double maxValue() const {return m_maxVal;}
    std::vector<double> values() const {return m_values;}
    
    void findBin (double x, double y,
                  unsigned int & xIndex, unsigned int & yIndex) const;
    double binContent(unsigned int xIndex, unsigned int yIndex) const;
    unsigned int binEntries(unsigned int xIndex, unsigned int yIndex) const;
    void binCenter (unsigned int xIndex, unsigned int yIndex,
                    double &xCenter, double &yCenter) const;
    double binWidthX (unsigned int xIndex = 0, unsigned int yIndex = 0) const;
    double binWidthY (unsigned int xIndex = 0, unsigned int yIndex = 0) const;                     
    void fill(double x, double y);
    void fill(double x, double y, double value);
    void fillBin(unsigned int xIndex, unsigned int yIndex);
    void fillBin(unsigned int xIndex, unsigned int yIndex, double value);
    void reset();
    
    // Terminal formatting.
    std::ostream& fillStream(std::ostream& os) const;
    friend std::ostream& operator<<(std::ostream& os, const pMap& map)
      {return map.fillStream(os);}
   
  private:
    
    pMap(); // declaring default constr. private, so it cannot be used
    void initialize();
    bool areCoordsInRange (unsigned int xIndex, unsigned int yIndex) const;
    unsigned int index (unsigned int xIndex, unsigned int yIndex) const;
    
    unsigned int m_nXbins;   // number of x bins
    double m_xmin;    // minimum of xAxis
    double m_xmax;    // maximum of xAxis
    double m_xWidth; // bin width along xAxis (-1 for non linear binning)
    bool m_xIsLinear; // true if the binning is linear
    unsigned int m_nYbins;   // number of y bins
    double m_ymin;    // minimum of yAxis
    double m_ymax;    // maximum of yAxis
    double m_yWidth; // bin width along yAxis (-1 for non linear binning)
    bool m_yIsLinear; // true if the binning is linear
    std::vector<double> m_xbinning;   // xAxis binning
    std::vector<double> m_ybinning;   // yAxis binning
    double m_minVal;    // minimum value stored
    double m_maxVal;    // maximum value stored
    std::vector<unsigned int> m_entries;  // entries in each bin
    std::vector<double> m_values;    // values in each bin
};


#endif // PMAP_H
