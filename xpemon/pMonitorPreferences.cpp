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
  xpolio::kIOManager->write(outputFile, m_socketPort);
  xpolio::kIOManager->write(outputFile, "//Refresh Interval//");
  xpolio::kIOManager->write(outputFile, m_refreshInterval);
  xpolio::kIOManager->write(outputFile, "//Zero Suppression Threshold//");
  xpolio::kIOManager->write(outputFile, m_zeroSuppressionThreshold);
  xpolio::kIOManager->write(outputFile, "//Minimum elongation//");
  xpolio::kIOManager->write(outputFile, m_minElongation);
  xpolio::kIOManager->write(outputFile, "//Maximum elongation//");
  xpolio::kIOManager->write(outputFile, m_maxElongation);
  xpolio::kIOManager->write(outputFile, "//Minimum cluster size//");
  xpolio::kIOManager->write(outputFile, m_minClusterSize);
  xpolio::kIOManager->write(outputFile, "//Maximum cluster size//");
  xpolio::kIOManager->write(outputFile, m_maxClusterSize);
  xpolio::kIOManager->write(outputFile, "//Minimum pulse height//");
  xpolio::kIOManager->write(outputFile, m_minPulseHeight);
  xpolio::kIOManager->write(outputFile, "//Maximum pulse height//");
  xpolio::kIOManager->write(outputFile, m_maxPulseHeight);
  xpolio::kIOManager->write(outputFile, "//Minimum window size//");
  xpolio::kIOManager->write(outputFile, m_minWindowSize);
  xpolio::kIOManager->write(outputFile, "//Maximum window size//");
  xpolio::kIOManager->write(outputFile, m_maxWindowSize);
  xpolio::kIOManager->write(outputFile, "//Display the box with the cuts//");
  xpolio::kIOManager->write(outputFile, m_showCuts);
  xpolio::kIOManager->closeOutputFile(outputFile);
}


/*!
*/
void pMonitorPreferences::readFromFile(std::string filePath)
{
  *xpollog::kInfo << "Reading user preferences from " << filePath <<
    "... " << endline;
  std::ifstream *inputFile = xpolio::kIOManager->openInputFile(filePath);
  xpolio::kIOManager->skipLine(inputFile);
  m_socketPort = xpolio::kIOManager->readUnsignedInt(inputFile);
  xpolio::kIOManager->skipLine(inputFile);
  m_refreshInterval = xpolio::kIOManager->readDouble(inputFile);
  xpolio::kIOManager->skipLine(inputFile);
  m_zeroSuppressionThreshold = xpolio::kIOManager->readUnsignedInt(inputFile);
  xpolio::kIOManager->skipLine(inputFile);
  m_minElongation = xpolio::kIOManager->readDouble(inputFile);
  xpolio::kIOManager->skipLine(inputFile);
  m_maxElongation = xpolio::kIOManager->readDouble(inputFile);
  xpolio::kIOManager->skipLine(inputFile);
  m_minClusterSize = xpolio::kIOManager->readInteger(inputFile);
  xpolio::kIOManager->skipLine(inputFile);
  m_maxClusterSize = xpolio::kIOManager->readInteger(inputFile);
  xpolio::kIOManager->skipLine(inputFile);
  m_minPulseHeight = xpolio::kIOManager->readInteger(inputFile);
  xpolio::kIOManager->skipLine(inputFile);
  m_maxPulseHeight = xpolio::kIOManager->readInteger(inputFile);
  xpolio::kIOManager->skipLine(inputFile);
  m_minWindowSize = xpolio::kIOManager->readInteger(inputFile);
  xpolio::kIOManager->skipLine(inputFile);
  m_maxWindowSize = xpolio::kIOManager->readInteger(inputFile);
  xpolio::kIOManager->skipLine(inputFile);
  m_showCuts = xpolio::kIOManager->readBool(inputFile);
  xpolio::kIOManager->closeInputFile(inputFile);
}


/*!
 */
std::ostream& pMonitorPreferences::fillStream(std::ostream& os) const
{
  os << xpedaqutils::title("User preferences", true);
  os << "Socket Port: " << m_socketPort << std::endl;
  os << "Refresh Interval: " << m_refreshInterval << std::endl;
  os << "Zero Suppression Threshold: " << m_zeroSuppressionThreshold
     << std::endl;
  os << "Min. elongation: " << m_minElongation << std::endl;
  os << "Max. elongation: " << m_maxElongation << std::endl;
  os << "Min. cluster size: " << m_minClusterSize << std::endl;
  os << "Max. cluster size: " << m_maxClusterSize << std::endl;
  os << "Min. pulse height: " << m_minPulseHeight << std::endl;
  os << "Max. pulse height: " << m_maxPulseHeight << std::endl;
  os << "Min. window size: " << m_minWindowSize << std::endl;
  os << "Max. window size: " << m_maxWindowSize << std::endl;
  os << xpedaqutils::hline();
  return os;
}
