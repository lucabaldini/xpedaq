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

#ifndef PHEADERFILE_H
#define PHEADERFILE_H

#include "xpolio.h"
#include "xpollog.h"

class pHeaderFile
{

 public:
  pHeaderFile();
  pHeaderFile(std::string inputFilePath);
  ~pHeaderFile() {;}
  void log();
  void readAscii(std::string inputFilePath);
  void writeBinary(std::string outputFilePath);
  void readBinary(std::string inputFilePath);

 protected:

 private:
  int m_versionNumber;
  int m_numEvents;
  float m_energy;
  float m_polarization;
  float m_delta;
  float m_epsilon;
  float m_x;
  float m_y;
  float m_z;
  char m_gasMixture[1000];
};

#endif
