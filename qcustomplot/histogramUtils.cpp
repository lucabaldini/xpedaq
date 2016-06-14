#include "histogramUtils.h"

bool isOrdered (const std::vector<double> & binning)
{
  std::vector<double>::const_iterator first = binning.begin();
  std::vector<double>::const_iterator last = binning.end();
  std::vector<double>::const_iterator next = first;
  while (++next != last)
  {
    if (*next <= *first)
      {return false;}
    ++first;
  }
  return true;
}

unsigned int findPosition(const std::vector<double> & v, double x)
{
  std::vector<double>::const_iterator it = 
                                       std::upper_bound (v.begin(), v.end(), x); 
  return static_cast<unsigned int> (std::distance(v.begin(), it) -1);
}
