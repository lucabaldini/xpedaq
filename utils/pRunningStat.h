#ifndef RUNNINGSTAT_H
#define RUNNINGSTAT_H

#include <iostream>

class pRunningStat
{
  
  public:
    
    pRunningStat() : m_numEntries(0),  m_currentMean(0.),
                     m_currentVariance(0.), m_prevMean(0.),  m_prevVariance(0.)
                     {};
    void fill(double value);
    int numValues() const {return m_numEntries;};
    double average() const;
    double variance() const;
    double rms() const;  
    friend std::ostream& operator<< (std::ostream &out,
                                     const pRunningStat &stat);


  private:
  
    int m_numEntries;
    double m_currentMean;
    double m_currentVariance;
    double m_prevMean;
    double m_prevVariance;

};

#endif // RUNNINGSTAT_H
