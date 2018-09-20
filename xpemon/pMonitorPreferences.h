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

#ifndef PMONITORPREFERENCES_H
#define PMONITORPREFERENCES_H

#include <iostream>

#include "xpedaqutils.h"
#include "xpollog.h"
#include "xpolio.h"


class pMonitorPreferences
{

 public:
  pMonitorPreferences();
  pMonitorPreferences(std::string filePath);
  ~pMonitorPreferences();

  //We will mostly read and write these fields as they are, so it make sense
  //to keep them public.
  unsigned int m_socketPort;
  double m_refreshInterval;
  unsigned int m_zeroSuppressionThreshold;
  double m_minElongation;
  double m_maxElongation;
  int m_minClusterSize;
  int m_maxClusterSize;
  int m_minPulseHeight;
  int m_maxPulseHeight;
  int m_minWindowSize;
  int m_maxWindowSize;
  double m_minSkewness;
  double m_maxSkewness;
  // Hidden options.
  // Show the widgets with the quality cuts in the monitor.
  bool m_showCuts;
  // Show the modulation plot (if false, this is replaced by the cluster size).
  bool m_showModulationPlot;
  // Disable the clustering and the reconstruction
  bool m_skipReconstruction;
  
  // Read/write to/from file.
  void writeToFile(std::string filePath);
  void readFromFile(std::string filePath);

  // Terminal formatting.
  std::ostream& fillStream(std::ostream& os) const;
  friend std::ostream& operator<<(std::ostream& os,
				  const pMonitorPreferences& preferences)
  {return preferences.fillStream(os);}

};

#endif //PMONITORPREFERENCES_H
