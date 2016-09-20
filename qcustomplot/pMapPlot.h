#ifndef MAPPLOT_H
#define MAPPLOT_H

#include "pCustomColorMapPlot.h"
#include "pMap.h"
#include "pHistogramOptions.h"

class pMapPlot : public pCustomColorMapPlot
{
  
  Q_OBJECT
  
  public:
  
    pMapPlot(const pMap* map, pColorMapOptions options = pColorMapOptions());
  
    unsigned int entries() const {return m_map -> entries();}
    double sum() const {return m_map -> sum();}
    
    // Terminal formatting.
    friend std::ostream& operator<<(std::ostream& os, const pMapPlot& mapPlot)
      {return mapPlot.m_map-> fillStream(os);}
     
  public slots:
  
    // Reset the axes to match the histogram dimension and set the
    // optimal color scale.
    virtual void resetView();
    
    // Update the displayed color map to the underlying pMap content
    void updateDisplay();
    
    // Clears data from the color map and resizes it to match the
    // underlying histogram.
    void reset();

  private:
    
    void setupDataMap();    
    //Align histogram bins and color map cells in a given range 
    void setMacthingRange(double xmin, double xmax,
                          double ymin, double ymax);
    
    const pMap *m_map;

};

#endif  //MAPPLOT_H
