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

  // Access methods.
  unsigned int socketPort() const {return m_socketPort;}
  double refreshInterval() const {return m_refreshInterval;}
  unsigned int zeroSuppressionThreshold() const
    {return m_zeroSuppressionThreshold;}
  double minElongation() const {return m_minElongation;}
  double maxElongation() const {return m_maxElongation;}
  
  // Set methods.
  void setSocketPort(unsigned int socketPort)
    {m_socketPort = socketPort;}
  void setRefreshInterval(double refreshInterval)
    {m_refreshInterval = refreshInterval;}
  void setZeroSuppressionThreshold(unsigned int zeroSuppressionThreshold)
    {m_zeroSuppressionThreshold = zeroSuppressionThreshold;}
  void setMinElongation(double elongation)
    {m_minElongation = elongation;}
  void setMaxElongation(double elongation)
    {m_maxElongation = elongation;}

  // Read/write to/from file.
  void writeToFile(std::string filePath);
  void readFromFile(std::string filePath);

  // Terminal formatting.
  std::ostream& fillStream(std::ostream& os) const;
  friend std::ostream& operator<<(std::ostream& os,
				  const pMonitorPreferences& preferences)
  {return preferences.fillStream(os);}

 private:
  unsigned int m_socketPort;
  double m_refreshInterval;
  unsigned int m_zeroSuppressionThreshold;
  double m_minElongation;
  double m_maxElongation;
};

#endif //PMONITORPREFERENCES_H
