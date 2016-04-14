/***********************************************************************
Copyright (C) 2007, 2008 by Luca Baldini (luca.baldini@pi.infn.it),
Johan Bregeon, Massimo Minuti and Gloria Spandre.

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

#include "pHeaderFile.h"

pHeaderFile::pHeaderFile()
{

}

pHeaderFile::pHeaderFile(std::string inputFilePath)
{
  readAscii(inputFilePath);
}

void pHeaderFile::log()
{
  *xpollog::kDebug << "Version number        : " << m_versionNumber << endline;
  *xpollog::kDebug << "Requested events      : " << m_numEvents << endline;
  *xpollog::kDebug << "Energy (keV)          : " << m_energy << endline;
  *xpollog::kDebug << "Polarization degree   : " << m_polarization << endline;
  *xpollog::kDebug << "Angle (delta, epsilon): " << "(" << m_delta << ", "
		   << m_epsilon << ")" << endline;
  *xpollog::kDebug << "Position (x, y, z)    : " << "(" << m_x << ", "
		   << m_y << ", " << m_z << ")" << endline;
  *xpollog::kDebug << "Gas mixture           : " << m_gasMixture << endline; 
}

void pHeaderFile::readAscii(std::string inputFilePath)
{
  *xpollog::kInfo << "Reading header from text file " << inputFilePath <<
    "... " << endline;
  std::ifstream *inputFile = xpolio::kIOManager->openInputFile(inputFilePath);
  *inputFile >> m_versionNumber;
  *inputFile >> m_numEvents;
  *inputFile >> m_energy;
  *inputFile >> m_polarization;
  *inputFile >> m_delta;
  *inputFile >> m_epsilon;
  *inputFile >> m_x;
  *inputFile >> m_y;
  *inputFile >> m_z;
  *inputFile >> m_gasMixture;
  xpolio::kIOManager->closeInputFile(inputFile);
  log();
}

void pHeaderFile::writeBinary(std::string outputFilePath)
{
  *xpollog::kInfo << "Writing header to binary file " << outputFilePath <<
    "... " << endline;
  std::ofstream *outputFile =
    xpolio::kIOManager->openOutputFile(outputFilePath, false, true);
  outputFile->write((char*)&m_versionNumber, sizeof(m_versionNumber));
  outputFile->write((char*)&m_numEvents, sizeof(m_numEvents));
  outputFile->write((char*)&m_energy, sizeof(m_energy));
  outputFile->write((char*)&m_polarization, sizeof(m_polarization));
  outputFile->write((char*)&m_delta, sizeof(m_delta));
  outputFile->write((char*)&m_epsilon, sizeof(m_epsilon));
  outputFile->write((char*)&m_x, sizeof(m_x));
  outputFile->write((char*)&m_y, sizeof(m_y));
  outputFile->write((char*)&m_z, sizeof(m_z));
  outputFile->write((char*)&m_gasMixture, sizeof(m_gasMixture));
  xpolio::kIOManager->closeOutputFile(outputFile);
}

void pHeaderFile::readBinary(std::string inputFilePath)
{
  *xpollog::kInfo << "Reading header from binary file " << inputFilePath <<
    "..." << endline;
  std::ifstream *inputFile =
    xpolio::kIOManager->openInputFile(inputFilePath, true);
  inputFile->read((char*)&m_versionNumber, sizeof(m_versionNumber));
  inputFile->read((char*)&m_numEvents, sizeof(m_numEvents));
  inputFile->read((char*)&m_energy, sizeof(m_energy));
  inputFile->read((char*)&m_polarization, sizeof(m_polarization));
  inputFile->read((char*)&m_delta, sizeof(m_delta));   
  inputFile->read((char*)&m_epsilon, sizeof(m_epsilon));
  inputFile->read((char*)&m_x, sizeof(m_x));
  inputFile->read((char*)&m_y, sizeof(m_y));
  inputFile->read((char*)&m_z, sizeof(m_z));
  inputFile->read((char*)&m_gasMixture, sizeof(m_gasMixture));
  xpolio::kIOManager->closeInputFile(inputFile);
  log();
}
