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

#include "xperegUserPreferences.h"


xperegUserPreferences::xperegUserPreferences(std::string filePath)
{
  readFromFile(filePath);
}



void xperegUserPreferences::writeToFile(std::string filePath)
{
  *xpollog::kInfo << "Writing user preferences to " << filePath <<
    "..." << endline;
  std::ofstream *outputFile = xpolio::kIOManager->openOutputFile(filePath);
  xpolio::kIOManager->write(outputFile, "//Pixel address x//");
  xpolio::kIOManager->write(outputFile, m_pixelAddressX);
  xpolio::kIOManager->write(outputFile, "//Pixel address y//");
  xpolio::kIOManager->write(outputFile, m_pixelAddressY);
  xpolio::kIOManager->write(outputFile, "//Configuration register///");
  xpolio::kIOManager->write(outputFile, m_configuration);
  xpolio::kIOManager->write(outputFile, "//Random shuffle//");
  xpolio::kIOManager->write(outputFile, m_randomShuffle);
  xpolio::kIOManager->write(outputFile, "//Number of readouts per poke//");
  xpolio::kIOManager->write(outputFile, m_readoutRepeat);
  xpolio::kIOManager->write(outputFile, "//Readout interval//");
  xpolio::kIOManager->write(outputFile, m_readoutInterval);
  xpolio::kIOManager->write(outputFile, "//Write data file//");
  xpolio::kIOManager->write(outputFile, m_dataFileEnabled);
  xpolio::kIOManager->write(outputFile, "//Output root folder//");
  xpolio::kIOManager->write(outputFile, m_outputFolder);
  xpolio::kIOManager->write(outputFile, "//Write log file//");
  xpolio::kIOManager->write(outputFile, m_logFileEnabled);
  xpolio::kIOManager->write(outputFile, "//Logger terminal level//");
  xpolio::kIOManager->write(outputFile, m_loggerTerminalLevel);
  xpolio::kIOManager->write(outputFile, "//Logger display level//");
  xpolio::kIOManager->write(outputFile, m_loggerDisplayLevel);
  xpolio::kIOManager->closeOutputFile(outputFile);
}


/*!
*/
void xperegUserPreferences::readFromFile(std::string filePath)
{
  *xpollog::kInfo << "Reading user preferences from " << filePath <<
    "... " << endline;
  std::ifstream *inputFile = xpolio::kIOManager->openInputFile(filePath);
  xpolio::kIOManager->skipLine(inputFile);
  m_pixelAddressX = xpolio::kIOManager->readUnsignedShort(inputFile);
  xpolio::kIOManager->skipLine(inputFile);
  m_pixelAddressY = xpolio::kIOManager->readUnsignedShort(inputFile);
  xpolio::kIOManager->skipLine(inputFile);
  m_configuration = xpolio::kIOManager->readUnsignedShort(inputFile);
  xpolio::kIOManager->skipLine(inputFile);
  m_randomShuffle = xpolio::kIOManager->readBool(inputFile);
  xpolio::kIOManager->skipLine(inputFile);
  m_readoutRepeat = xpolio::kIOManager->readInteger(inputFile);
  xpolio::kIOManager->skipLine(inputFile);
  m_readoutInterval = xpolio::kIOManager->readInteger(inputFile);
  xpolio::kIOManager->skipLine(inputFile);
  m_dataFileEnabled = xpolio::kIOManager->readBool(inputFile);
  xpolio::kIOManager->skipLine(inputFile);
  m_outputFolder = xpolio::kIOManager->readLine(inputFile);
  xpolio::kIOManager->skipLine(inputFile);
  m_logFileEnabled = xpolio::kIOManager->readBool(inputFile);
  xpolio::kIOManager->skipLine(inputFile);
  m_loggerTerminalLevel = xpolio::kIOManager->readInteger(inputFile);
  xpolio::kIOManager->skipLine(inputFile);
  m_loggerDisplayLevel = xpolio::kIOManager->readInteger(inputFile);  
  xpolio::kIOManager->closeInputFile(inputFile);
}


/*!
 */
std::ostream& xperegUserPreferences::fillStream(std::ostream& os) const
{
  os << xpedaqutils::title("User preferences", true);
  os << "Pixel address x: " << m_pixelAddressX << std::endl;
  os << "Pixel address y: " << m_pixelAddressY << std::endl;
  os << "Configuration register: " << m_configuration << std::endl;
  os << "Random shuffle: " << m_randomShuffle << std::endl;
  os << "Number of readouts per poke: " << m_readoutRepeat << std::endl;
  os << "Readout interval: " << m_readoutInterval << std::endl;
  os << "Write data file: " << m_dataFileEnabled << std::endl;
  os << "Output root folder: " << m_outputFolder << std::endl;
  os << "Write log file: " << m_logFileEnabled << std::endl;
  os << "Logger terminal level: " << m_loggerTerminalLevel << std::endl;
  os << "Logger display level: " << m_loggerDisplayLevel << std::endl;
  os << xpedaqutils::hline();
  return os;
}
