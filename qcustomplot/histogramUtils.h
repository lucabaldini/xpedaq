#ifndef HISTOGRAMUTILS_H
#define HISTOGRAMUTILS_H

#include <vector>
#include <algorithm>

bool isOrdered (const std::vector<double> & binning);

enum class HistogramError
{
  INVALID_NUMBER_OF_BINS,
  INVALID_BOUNDARIES,
  INVALID_BINNING,
  BIN_OUT_OF_RANGE,  
  VALUE_LOWER_THAN_AXIS_RANGE,
  VALUE_GREATER_THAN_AXIS_RANGE
};

unsigned int findPosition(const std::vector<double> & v, double x);

#endif // HISTOGRAMUTILS_H
