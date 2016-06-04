#include "pMapPlot.h"

pMapPlot::pMapPlot(unsigned int nXbins, double xmin, double xmax,
                   unsigned int nYbins, double ymin, double ymax,
                   pColorMapOptions options) : pCustomColorMapPlot (options)
{
  m_map = new pMap(nXbins, xmin, xmax, nYbins, ymin, ymax);
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
     first edge of half a bin forward and the last edge of half a bin backward,
     in order to recover a matching with the underlying histogram (i.e. to 
     have cells centered at the center of the corresponding bins).
  */
  double xPad = (m_map -> binWidthX())/2.;
  double yPad = (m_map -> binWidthY())/2.;
  setRange(xmin + xPad, xmax - xPad, ymin + yPad, ymax - yPad); 
}
                              

void pMapPlot::fillBin(unsigned int xbin, unsigned int ybin, double value)
{ 
  m_map -> fillBin(xbin, ybin, value);
  setCellContent (xbin, ybin, m_map -> binContent(xbin, ybin));
  rescaleDataRange();
}


void pMapPlot::fillBin(unsigned int xbin, unsigned int ybin)
{
  fillBin(xbin, ybin, 1.);
}


void pMapPlot::fill(double x, double y, double value)
{ 
  m_map -> fill(x, y, value);
  /* To fill the ColorDataMap we could use setDataContent(), but since we need
     to call findBin() anyway (to retrive the content of the corresponding
     histogram bin) it is worth using setCellContent() instead, which is faster.
     Note that this assumes matching between cell numbering and bin numbering,
     which should always be true by construction.
  */
  unsigned int xbin, ybin;
  m_map -> findBin(x, y, xbin, ybin);
  setCellContent (xbin, ybin, m_map -> binContent(xbin, ybin));
  rescaleDataRange();
}


void pMapPlot::fill(double x, double y)
{
  fill(x, y, 1.);
}


void pMapPlot::resetData()
{
  m_map -> reset();
  clearMap();
}


void pMapPlot::reset()
{
  resetData();
  setupDataMap();
  replot();
}

