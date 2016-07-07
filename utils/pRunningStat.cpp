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


void pRunningStat::reset()
{
  m_numEntries = 0;
  m_currentMean = 0;
  m_currentVariance = 0;
  m_prevMean = 0;
  m_prevVariance = 0;
}


std::ostream& operator<< (std::ostream &out, const pRunningStat &stat)
{
  out << "Sample mean = " << stat.average()
      << " ( " << stat.numValues() << " entries)";
  try
  {
    out << ", sample standard deviation = " << stat.rms();
  }
  catch (int error)
  {
    out << ", sample standard deviation = 0.";
  }
  return out;
}

