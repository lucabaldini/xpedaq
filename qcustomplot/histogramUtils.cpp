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
