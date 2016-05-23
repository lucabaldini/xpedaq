#include "pulseHeightPlot.h"
#include <iostream>

PulseHeightPlot::PulseHeightPlot(unsigned int nBins,
                                 double xmin, double xmax):
                                 m_nBins(nBins),m_xmin(xmin), m_xmax(xmax)
{
  if (m_nBins == 0)
  {
    m_nBins = 1;  //A bit rude, proper error handling needed
  }
  setInteractions(QCP::iRangeDrag|QCP::iRangeZoom);
  m_pulseHeightHist = new QCPBars(this->xAxis,
                                  this->yAxis);
  this -> addPlottable(m_pulseHeightHist);
  m_pulseHeightHist->keyAxis()->setRange(m_xmin, m_xmax);
  m_pulseHeightHist->setWidth(binWidth());
  for (unsigned int ibin = 0; ibin < m_nBins; ++ibin)
  {
    m_keys.push_back(binCenter(ibin));
    m_values.push_back(0.);
  }
}


double PulseHeightPlot::binWidth()
{
  return (m_xmax - m_xmin)/m_nBins;
}


/* First bin is n. zero (not 1 as in ROOT) */
double PulseHeightPlot::binCenter(unsigned int bin)
{
  return m_xmin + (bin + 0.5) * binWidth();
}


unsigned int PulseHeightPlot::findBin(double value)
{
  if ((value < m_xmin) || (value > m_xmax))
  {
    return 0;
  }
  return (unsigned int)((value-m_xmin)/binWidth());
}


void PulseHeightPlot::addPoint(int pulseHeight)
{
  /* Apparently QCPBars does not provide a method for incrementing the content
     of a bin. The default beahviour, when the function addData() is called on
     a key which already exists, is to create another entry for the same key.
     So we need to do this manually: keep a record of the content of the bins
     and, each time a bin is incremented, remove the old value and recreate
     the pair key:value with the updated content.
  */ 
  unsigned int bin = findBin(static_cast<double>(pulseHeight));
  double key = binCenter(bin); 
  m_values.at(bin) += 1.;  
  double tolerance = 1.e-5; // Make sure we remove the key
  m_pulseHeightHist -> removeData(key-tolerance, key+tolerance);
  m_pulseHeightHist -> addData(key, log10(m_values.at(bin)));
}
