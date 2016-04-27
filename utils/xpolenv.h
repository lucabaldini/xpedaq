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

#ifndef XPOLENV_H
#define XPOLENV_H

#include <iostream>
#include <cstdlib>

/*! \brief Namespace containing all the relevant information about the
  environment in which the application is running.
*/

namespace xpolenv{
  extern std::string kDaqRootDirPath;
  extern std::string kNullPath;
  extern std::string kDirSeparator;
  extern std::string kDaqConfigDirPath;
  extern std::string kDaqGuiDirPath;
  extern std::string kDaqPixmapsDirPath;
  extern std::string getEnvironmentVariable(std::string varName);
  extern std::string getEnvironmentVariable(std::string varName,
					    std::string defaultPath);
  extern std::string join(std::string path1, std::string path2);
  extern std::string appendToDaqRoot(std::string path);
}

#endif
