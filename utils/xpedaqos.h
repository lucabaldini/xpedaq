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


#ifndef XPEDAQOS_H
#define XPEDAQOS_H


#include <iostream>
#include <cstdlib>
#include <QDir>

#include "xpollog.h"


namespace xpedaqos{
  extern std::string kSeparator;
  extern std::string environmentalVariable(std::string name);
  extern std::string join(std::string p1, std::string p2);
  extern std::string join(std::string p1, std::string p2, std::string p3);
  extern std::string join(std::string p1, std::string p2, std::string p3,
			  std::string p4);
  extern std::string join(std::string p1, std::string p2, std::string p3,
			  std::string p4, std::string p5);
  extern std::string rjoin(std::string p1);
  extern std::string rjoin(std::string p1, std::string p2);
  extern std::string rjoin(std::string p1, std::string p2, std::string p3);
  extern std::string rjoin(std::string p1, std::string p2, std::string p3,
			   std::string p4);
  extern void mkdir(std::string path);
  extern bool fileExists(std::string path);
  extern bool folderExists(std::string path);
  extern bool copyFile(std::string src, std::string dest);
  extern bool nameEndsWith(std::string filePath, std::string ending);
}

#endif //XPEDAQOS_H

