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

#include <fstream>

#include "xpollog.h"
#include "pDataBlock.h"


int main(int argn, char *argv[])
{
  std::string filePath = "../../data/test_data_correct.mdat";
  //std::string filePath = "../../data/test_data_error.mdat";
  *xpollog::kInfo << "Opening input file " << filePath << "..." << endline;
  std::ifstream inputFile(filePath.c_str(), std::ios::binary);
  std::streampos begin, end;
  begin = inputFile.tellg();
  inputFile.seekg (0, std::ios::end);
  end = inputFile.tellg();
  inputFile.seekg(0, std::ios::beg);
  const unsigned long fileSize = end - begin;
  *xpollog::kInfo << "Input file is " << fileSize << " bytes long." << endline;

  unsigned char buffer[fileSize];
  inputFile.read((char*)buffer, fileSize);
  pDataBlock dataBlock(buffer, sizeof(buffer));
  std::cout << dataBlock << std::endl;
  inputFile.close();
  return 0;
}
