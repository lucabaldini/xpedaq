#ifndef HISTOGRAMUTILS_H
#define HISTOGRAMUTILS_H


#include <vector>

bool isOrdered (const std::vector<double> & binning);

enum class HistogramError
{
  INVALID_BIN_NUMBER,
  INVALID_BOUNDARIES,
  INVALID_BINNING,
  VALUE_OUT_OF_RANGE
};


#endif // HISTOGRAMUTILS_H
