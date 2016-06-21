#ifndef MAPPLOT_H
#define MAPPLOT_H

#include "pCustomColorMapPlot.h"
#include "pMap.h"
#include "pHistogramOptions.h"

class pMapPlot : public pCustomColorMapPlot
{
  
  Q_OBJECT
  
  public:
  
    pMapPlot(unsigned int nXbins, double xmin, double xmax,
             unsigned int nYbins, double ymin, double ymax,
             pColorMapOptions options = pColorMapOptions());
  
    unsigned int entries() const {return m_map -> entries();}
    double sum() const {return m_map -> sum();}
    void fill(double x, double y);
    void fill(double x, double y, double value);
    void fillBin(unsigned int xIndex, unsigned int yIndex);
    void fillBin(unsigned int xIndex, unsigned int yIndex, double value);  
    virtual void updateData (const std::vector<double> &values);

    // clears data both from the color map and from the histogram.
    // resizes the color map to match the underlying histogram.
    void reset();
    
    // resets the data in the underlying histogram and in the color map
    void resetData();
    
    // Terminal formatting.
    friend std::ostream& operator<<(std::ostream& os, const pMapPlot& mapPlot)
      {return mapPlot.m_map-> fillStream(os);}
     
  private:
    
    void setupDataMap();    
    //Align histogram bins and color map cells in a given range 
    void setMacthingRange(double xmin, double xmax,
                          double ymin, double ymax);
    
    pMap *m_map;

};

#endif  //MAPPLOT_H
