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


/*!
  Points to <tt>{\ref kDaqRootDirPath}/config/</tt>.
*/
std::string xpolenv::kDaqConfigDirPath =
  xpolenv::appendToDaqRoot("config");
/*!
  Points to <tt>{\ref kDaqRootDirPath}/gui/</tt>.
*/
std::string xpolenv::kDaqGuiDirPath =
  xpolenv::appendToDaqRoot("gui");
/*!
  Points to <tt>{\ref kDaqGuiDirPath}/pixmaps/</tt>.
*/
std::string xpolenv::kDaqPixmapsDirPath =
  xpolenv::join(xpolenv::kDaqGuiDirPath, "pixmaps");
/*!
  Points to the environmental variable <tt>XPEDAQ_DATA</tt> if it is set,
  to <tt>{\ref kDaqRootDirPath}/data/</tt> otherwise.
*/
std::string xpolenv::kDaqDataDirPath =
  xpolenv::getEnvironmentVariable("XPEDAQ_DATA",
				  xpolenv::appendToDaqRoot("data"));

/*! \param varName
  The name of the environmental variable.

  If the variable is not set the program exit with an error message.
*/
std::string xpolenv::getEnvironmentVariable(std::string varName)
{
  char *variable = ::getenv(varName.c_str());
  if (variable == NULL)
  {
    std::cout << "Environmental variable " << varName << " not defined."
	      << std::endl;
    exit(1);
  } else {
    return std::string(variable);
  }
}

/*! \param varName
  The name of the environmental variable.
  \param defaultPath
  The path which is returned if the environmental variable is not defined.

  Used to retrieve the optional environmental variables.
*/
std::string xpolenv::getEnvironmentVariable(std::string varName,
					    std::string defaultPath)
{
  char *variable = std::getenv(varName.c_str());
  if (variable == NULL)
  {
    return defaultPath;
  } else {
    return std::string(variable);
  }
}

/*! \param path1
  The first part of the path to be concatenated.
  \param path2
  The second part of the path to be concatenated.
*/
std::string xpolenv::join(std::string path1, std::string path2)
{
  return path1 + xpolenv::kDirSeparator + path2; 
}

/*! \param path
  The path to be appended to <tt>XPEDAQ_ROOT</tt>.
*/
std::string xpolenv::appendToDaqRoot(std::string path)
{
  return join(xpolenv::kDaqRootDirPath, path);
}
