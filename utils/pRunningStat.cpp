#include "pRunningStat.h"
#include <math.h>

void pRunningStat::fill(double value)
{
  m_numEntries += 1;
  if (m_numEntries == 1)
  {
    m_currentMean = value;
    m_prevMean = value;
  }
  else
  {
    m_currentMean = m_prevMean + (value - m_prevMean)/m_numEntries;
    m_currentVariance = m_prevVariance + (value - m_prevMean) 
                                                    * (value - m_currentMean);
    m_prevMean = m_currentMean;
    m_prevVariance = m_currentVariance;
  }
}


double pRunningStat::average() const
{

  if (m_numEntries  < 1)
    throw -1;
  return m_currentMean;
}


double pRunningStat::variance() const
{
  if (m_numEntries < 2)
    throw -1;
  return m_currentVariance/(m_numEntries - 1.);
}


double pRunningStat::rms() const
{
  if (m_numEntries < 2)
    throw -1;
  return sqrt(this->variance());
}  


std::ostream& operator<< (std::ostream &out, const pRunningStat &stat)
{
  try
  {
    out << "Sample mean = " << stat.average() <<
      ", sample standard deviation = " << stat.rms() << " ( " <<
        stat.numValues() << " entries)"; 
  }
  catch (int error)
  {
    out << "Empty 'pRunningStat' object";
  }
  return out;
}
