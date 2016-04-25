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
#include <iostream>

int main(int argn, char *argv[])
{
  if (argn > 1)
    {
      std::string option = argv[1];
      if (option == "-w")
	{
	  pHeaderFile headerFile("../../data/header.txt");
	  headerFile.writeBinary("test.dat");
	}
      else if (option == "-r")
	{
	  pHeaderFile headerFile;
	  headerFile.readBinary("test.dat");
	}
    } else {
      pHeaderFile headerFile("../../data/header.txt");
      headerFile.writeBinary("test.dat");
      headerFile.readBinary("test.dat");
    }
  return 0;
}
