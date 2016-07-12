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

#include "pMonitorPreferences.h"

pMonitorPreferences::pMonitorPreferences()
{
  // Do nothing constructor.
}

pMonitorPreferences::pMonitorPreferences(std::string filePath)
{
  readFromFile(filePath);
}

pMonitorPreferences::~pMonitorPreferences()
{
  // Do nothing destructor.
}

void pMonitorPreferences::writeToFile(std::string filePath)
{
  *xpollog::kInfo << "Writing user preferences to " << filePath <<
    "..." << endline;
  std::ofstream *outputFile = xpolio::kIOManager->openOutputFile(filePath);
  xpolio::kIOManager->write(outputFile, "//Socket Port//");
  xpolio::kIOManager->write(outputFile, socketPort());
  xpolio::kIOManager->write(outputFile, "//Refresh Interval//");
  xpolio::kIOManager->write(outputFile, refreshInterval());
  xpolio::kIOManager->write(outputFile, "//Zero Suppression Threshold//");
  xpolio::kIOManager->write(outputFile, zeroSuppressionThreshold());
  xpolio::kIOManager->closeOutputFile(outputFile);
}

void pMonitorPreferences::readFromFile(std::string filePath)
{
  *xpollog::kInfo << "Reading user preferences from " << filePath <<
    "... " << endline;
  std::ifstream *inputFile = xpolio::kIOManager->openInputFile(filePath);
  xpolio::kIOManager->skipLine(inputFile);
  setSocketPort(xpolio::kIOManager->readUnsignedInt(inputFile));
  xpolio::kIOManager->skipLine(inputFile);
  setRefreshInterval(xpolio::kIOManager->readDouble(inputFile));
  xpolio::kIOManager->skipLine(inputFile);
  setZeroSuppressionThreshold(xpolio::kIOManager->readUnsignedInt(inputFile));
  xpolio::kIOManager->closeInputFile(inputFile);
}


/*!
 */
std::ostream& pMonitorPreferences::fillStream(std::ostream& os) const
{
  os << xpedaqutils::title("User preferences", true);
  os << "Socket Port: " << socketPort() << std::endl;
  os << "Refresh Interval: " << refreshInterval() << std::endl;
  os << "Zero Suppression Threshold: " << zeroSuppressionThreshold() << std::endl;
  os << xpedaqutils::hline();
  return os;
}
