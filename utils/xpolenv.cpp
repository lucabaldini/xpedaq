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

#include "xpolenv.h"


std::string xpolenv::kDaqRootDirPath =
  xpolenv::getEnvironmentVariable("XPEDAQ_ROOT");

std::string xpolenv::kNullPath = "";

#ifdef  __WIN32__
std::string xpolenv::kDirSeparator = "\\";
#endif

#ifdef  __linux__
std::string xpolenv::kDirSeparator = "/";
#endif

std::string xpolenv::kDaqConfigDirPath = xpolenv::appendToDaqRoot("config");

std::string xpolenv::kDaqGuiDirPath = xpolenv::appendToDaqRoot("gui");

std::string xpolenv::kDaqPixmapsDirPath =
  xpolenv::join(xpolenv::kDaqGuiDirPath, "pixmaps");


std::string xpolenv::getEnvironmentVariable(std::string varName)
{
  char *variable = std::getenv(varName.c_str());
  if (variable == NULL) {
    std::cout << "Environmental variable " << varName << " not defined."
	      << std::endl;
    exit(1);
  } else {
    return std::string(variable);
  }
}

std::string xpolenv::getEnvironmentVariable(std::string varName,
					    std::string defaultPath)
{
  char *variable = std::getenv(varName.c_str());
  if (variable == NULL) {
    return defaultPath;
  } else {
    return std::string(variable);
  }
}

std::string xpolenv::join(std::string path1, std::string path2)
{
  return path1 + xpolenv::kDirSeparator + path2; 
}

std::string xpolenv::appendToDaqRoot(std::string path)
{
  return join(xpolenv::kDaqRootDirPath, path);
}
