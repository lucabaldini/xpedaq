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


#include "xpedaqos.h"


#ifdef  __WIN32__
std::string xpedaqos::kSeparator = "\\";
#else
std::string xpedaqos::kSeparator = "/";
#endif


std::string xpedaqos::environ(std::string name)
{
  char *path = std::getenv(name.c_str());
  if (path == NULL) {
    std::cout << "Environmental variable " << name << " not defined."
	      << std::endl;
    exit(1);
  } else {
    return std::string(path);
  }
}

std::string xpedaqos::join(std::string p1, std::string p2)
{
  return p1 + xpedaqos::kSeparator + p2;
}

std::string xpedaqos::join(std::string p1, std::string p2, std::string p3)
{
  return xpedaqos::join(join(p1, p2), p3);
}

std::string xpedaqos::join(std::string p1, std::string p2, std::string p3,
			   std::string p4)
{
  return xpedaqos::join(join(p1, p2, p3), p4);
}

std::string xpedaqos::join(std::string p1, std::string p2, std::string p3,
			   std::string p4, std::string p5)
{
  return xpedaqos::join(join(p1, p2, p3, p4), p5);
}

void xpedaqos::mkdir(std::string path)
{
  if (!QDir(QString::fromStdString(path)).exists()) {
    *xpollog::kInfo << "Creating " << path << "..." << endline;
    QDir().mkpath(QString::fromStdString(path));
  }
}
