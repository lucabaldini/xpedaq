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

#include "pedFile.h"

/*
*/
PedFile::PedFile(std::string filePath) :
  m_fileType (undefined)
{
  if (!xpedaqos::fileExists(filePath)) {
    *xpollog::kError << "File not found: " << filePath
                     << endline;
    exit(1);
  }
  m_inputFile = xpolio::kIOManager-> openInputFile(filePath);
  int fileErr = m_inputFile->bad();; //file error status
  if (fileErr) {
    *xpollog::kError << "Could not open input file. Exiting..."
                    << endline;
    exit(1);
  }
}


/*
*/
PedFile::~PedFile()
{
  xpolio::kIOManager->closeInputFile(m_inputFile);
}


/*
*/
std::streampos PedFile::fileSize() const
{
  goToStart(); // move the read cursor to the beginning of the file
  std::streampos begin = m_inputFile->tellg(); 
  m_inputFile->seekg(0, std::ios::end); // move the read cursor to the end
  return m_inputFile->tellg() - begin; // return the difference
}


/*
*/
void PedFile::goToStart() const
{
  m_inputFile->clear();
  m_inputFile->seekg(0, std::ios::beg);
}


/**** END OF CLASS PEDFILE ****/

/*
*/
PedFile::inputFileType identifyFileType(std::string filePath)
{
  if (xpedaqos::nameEndsWith(filePath, ".pmap")){
    return PedFile::pedmapType;
  } else if (xpedaqos::nameEndsWith(filePath, ".mdat")){
    return PedFile::dataType;
  } else {
    return PedFile::unknownType;
  }
}

