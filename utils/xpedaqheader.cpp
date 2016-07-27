#include "xpedaqheader.h"

std::ostream& pFileHeader_v1::fillStream(std::ostream& os) const
{
  os << startWord << version << size << runId << stationId << startSeconds
     << readoutMode;
  for (int i =0; i < NUM_READOUT_CLUSTERS; ++i) {os << thresholdDac[i];}
  os << bufferMode << clockFrequency << clockShift << numPedSamples
     << pedSampleDelay << trgEnableDelay << minWindowSize << maxWindowSize
     << comment;
  return os;
}
