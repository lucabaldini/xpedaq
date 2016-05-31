#ifndef PEVENT_H
#define PEVENT_H

#include <vector>
#include <iostream>
#include <sstream>
#include <iomanip>

namespace event
{
  typedef std::vector<unsigned int> Adc_vec_t;
}

class pEvent
{
  
  public:
    
    pEvent(unsigned int xmin, unsigned int xmax, unsigned int ymin,
           unsigned int ymax, unsigned int bufferId,
           event::Adc_vec_t adcCounts);
  public:
    
    const event::Adc_vec_t & adcCounts () const { return m_adcCounts;}     // vector of all the pulse heights
    unsigned int xmin() const {return m_xmin;}
    unsigned int xmax() const {return m_xmax;}
    unsigned int ymin() const {return m_ymin;}
    unsigned int ymax() const {return m_ymax;}
    unsigned int bufferId() const {return m_bufferId;}
    unsigned int nColumns() const;
    unsigned int nRows() const;
    unsigned int totalAdcCounts() const; // sum of all pulse heights
    
    // Terminal formatting.
    std::ostream& fillStream(std::ostream& os) const;
    friend std::ostream& operator<<(std::ostream& os, const pEvent& event)
    {return event.fillStream(os);}
    
    
  private:
    
    unsigned int m_xmin;
    unsigned int m_xmax;
    unsigned int m_ymin;
    unsigned int m_ymax;
    unsigned int m_bufferId;
    event::Adc_vec_t m_adcCounts;
    
};

#endif //PEVENT_H
