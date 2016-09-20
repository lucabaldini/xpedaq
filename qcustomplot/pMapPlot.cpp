#include "pMapPlot.h"

pMapPlot::pMapPlot(const pMap* map, pColorMapOptions options):
                   pCustomColorMapPlot (options),
                   m_map(map)
{
  // Update the underlying ColorMapData to match the histogram binning
  setupDataMap();
}


void pMapPlot::setupDataMap()
{
  setMacthingRange(m_map -> xMin(), m_map -> xMax(),
                   m_map -> yMin(), m_map -> yMax());
  setSize(m_map -> nXbins(), m_map -> nYbins());
  rescaleAxes();
}


void pMapPlot::setMacthingRange(double xmin, double xmax,
                                double ymin, double ymax)
{
  /* In a QCPColorMap the first cell is centered on the lower range boundary
     and the last cell on the upper range boundary. Thus, we need to shift the
     first edge of half a bin forward and the last edge of half a bin
     backward, in order to recover a matching with the underlying histogram
     (i.e. to have cells centered at the center of the corresponding bins).
  */
  double xPad = (m_map -> binWidthX())/2.;
  double yPad = (m_map -> binWidthY())/2.;
  setRange(xmin + xPad, xmax - xPad, ymin + yPad, ymax - yPad); 
}
                              

void pMapPlot::resetView()
{
  setMacthingRange(m_map -> xMin(), m_map -> xMax(),
                   m_map -> yMin(), m_map -> yMax());
  m_colorMap -> rescaleDataRange();
  replot();
}


void pMapPlot::updateDisplay()
{
  double x,y;
  for (unsigned int i=0; i < (m_map -> nXbins()); ++i){
      for (unsigned int j=0; j < (m_map -> nYbins()); ++j){
          m_map -> binCenter (i, j, x, y);
          setDataContent(x, y, m_map -> binContent(i, j));
      }
  }
}


void pMapPlot::reset()
{
  //m_map -> reset();
  clearMap();
  setupDataMap();
}

