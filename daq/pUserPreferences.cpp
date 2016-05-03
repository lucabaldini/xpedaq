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

#include "pUserPreferences.h"

pUserPreferences::pUserPreferences()
{
  // Do nothing constructor.
}

pUserPreferences::pUserPreferences(std::string filePath)
{
  readFromFile(filePath);
}

pUserPreferences::~pUserPreferences()
{
  // Do nothing destructor.
}

void pUserPreferences::writeToFile(std::string filePath)
{
  *xpollog::kInfo << "Writing user preferences to " << filePath <<
    "..." << endline;
  std::ofstream *outputFile = xpolio::kIOManager->openOutputFile(filePath);
  xpolio::kIOManager->write(outputFile, "//Visualization mode//");
  xpolio::kIOManager->write(outputFile, visualizationMode());
  xpolio::kIOManager->write(outputFile, "//Write data file//");
  xpolio::kIOManager->write(outputFile, dataFileEnabled());
  xpolio::kIOManager->write(outputFile, "//Output root folder//");
  xpolio::kIOManager->write(outputFile, outputFolder());
  xpolio::kIOManager->write(outputFile, "//Multicast enabled//");
  xpolio::kIOManager->write(outputFile, multicastEnabled());
  xpolio::kIOManager->write(outputFile, "//Multicast address//");
  xpolio::kIOManager->write(outputFile, multicastAddress());
  xpolio::kIOManager->write(outputFile, "//Multicast port//");
  xpolio::kIOManager->write(outputFile, multicastPort());
  xpolio::kIOManager->write(outputFile, "//Write log file//");
  xpolio::kIOManager->write(outputFile, logFileEnabled());
  xpolio::kIOManager->write(outputFile, "//Logger terminal level//");
  xpolio::kIOManager->write(outputFile, loggerTerminalLevel());
  xpolio::kIOManager->write(outputFile, "//Logger display level//");
  xpolio::kIOManager->write(outputFile, loggerDisplayLevel());
  xpolio::kIOManager->closeOutputFile(outputFile);
}

void pUserPreferences::readFromFile(std::string filePath)
{
  *xpollog::kInfo << "Reading user preferences from " << filePath <<
    "... " << endline;
  std::ifstream *inputFile = xpolio::kIOManager->openInputFile(filePath);
  xpolio::kIOManager->skipLine(inputFile);
  setVisualizationMode(xpolio::kIOManager->readUnsignedShort(inputFile));
  xpolio::kIOManager->skipLine(inputFile);
  enableDataFile(bool(xpolio::kIOManager->readUnsignedShort(inputFile)));
  xpolio::kIOManager->skipLine(inputFile);
  setOutputFolder(xpolio::kIOManager->readLine(inputFile));
  xpolio::kIOManager->skipLine(inputFile);
  enableMulticast(bool(xpolio::kIOManager->readUnsignedShort(inputFile)));
  xpolio::kIOManager->skipLine(inputFile);
  setMulticastAddress(xpolio::kIOManager->readLine(inputFile));
  xpolio::kIOManager->skipLine(inputFile);
  setMulticastPort(xpolio::kIOManager->readInteger(inputFile));
  xpolio::kIOManager->skipLine(inputFile);
  enableLogFile(bool(xpolio::kIOManager->readUnsignedShort(inputFile)));
  xpolio::kIOManager->skipLine(inputFile);
  setLoggerTerminalLevel(xpolio::kIOManager->readInteger(inputFile));
  xpolio::kIOManager->skipLine(inputFile);
  setLoggerDisplayLevel(xpolio::kIOManager->readInteger(inputFile));
  xpolio::kIOManager->closeInputFile(inputFile);
}


/*!
 */
std::ostream& pUserPreferences::fillStream(std::ostream& os) const
{
  os << xpedaqutils::title("User preferences", true);
  os << "Visualization mode: " << visualizationMode() << std::endl;
  os << "Output data file enabled: " << dataFileEnabled() << std::endl;
  os << "Output root folder: " << outputFolder() << std::endl;
  os << "Multicast enabled: " << multicastEnabled() << std::endl;
  os << "Multicast address: " << multicastAddress() << std::endl;
  os << "Multicast port: " << multicastPort() << std::endl;
  os << "Log file enabled: " << logFileEnabled() << std::endl;
  os << "Logger terminal level: " << loggerTerminalLevel() << std::endl;
  os << "Logger display level: " << loggerDisplayLevel() << std::endl;
  os << xpedaqutils::hline();
  return os;
}
