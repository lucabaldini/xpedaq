/***********************************************************************
Copyright (C) 2007--2016 the X-ray Polarimetry Explorer (XPE) team.

For the license terms see the file LICENSE, distributed along with this
software.

This program is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation; either version 2 of the License, or (at your
option) any later version.

This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the Free Software Foundation Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
***********************************************************************/

#define FILE_HEADER_START_WORD 0xefff
#define FILE_HEADER_VERSION 1


#ifndef XPEDAQHEADER_H
#define XPEDAQHEADER_H


#include "xpoldetector.h"
#include <stdint.h>


struct pFileHeader_v1
{
  // Preamble.
  uint16_t startWord;
  uint16_t version;
  uint32_t size;
  // Run-specific stuff.
  int32_t runId;
  int32_t stationId;
  int64_t startSeconds;
  // Configuration book-keeping.
  uint16_t readoutMode;
  uint16_t thresholdDac[NUM_READOUT_CLUSTERS];
  uint16_t bufferMode;
  uint16_t clockFrequency;
  uint16_t clockShift;
  uint16_t numPedSamples;
  uint16_t pedSampleDelay;
  uint16_t trgEnableDelay;
  uint16_t minWindowSize;
  uint16_t maxWindowSize;
  std::string comment;
  
  // Output formatting
  std::ostream& fillStream(std::ostream& os) const
  {
    os << startWord << version << size << runId << stationId << startSeconds
       << readoutMode;
    for (int i =0; i < NUM_READOUT_CLUSTERS; ++i) {os << thresholdDac[i];}
    os << bufferMode << clockFrequency << clockShift << numPedSamples
       << pedSampleDelay << trgEnableDelay << minWindowSize << maxWindowSize
       << comment;
    return os;
  }
  friend std::ostream& operator<<(std::ostream& os,
                                  const pFileHeader_v1 &header)
    {return header.fillStream(os);}
};


#endif //XPEDAQHEADER_H
