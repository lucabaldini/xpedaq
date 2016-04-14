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

#ifndef XPOLENV_H
#define XPOLENV_H

#include <iostream>
#include <cstdlib>

/*! \brief Namespace containing all the relevant information about the
  environment in which the application is running.

  The <tt>XPOL_DAQ_ROOT</tt> environmental variable must be defined and
  point to the root of the package installation.
  There are two other environmental variables which can be optionally set:
  <tt>XPOL_DAQ_DATA</tt> identifies the folder where the data are written and
  <tt>XPOL_DAQ_LOG</tt> defines the folder for the log files. If not set,
  those two paths get their default values (see following details).
 */


namespace xpolenv{
  /*! \brief Path to the base installation directory.*/
  extern std::string kDaqRootDirPath;
  /*! \brief Dummy empty path (actually an empty string).*/
  extern std::string kNullPath;
  /*! \brief Directory separator (either "/" or "\" depending on the os).*/
  extern std::string kDirSeparator;
  /*! \brief Path to the text file containing the runId.*/
  extern std::string kDefaultRunIdFilePath;
  /*! \brief Path to the file containing the default preferences.*/
  extern std::string kDefaultPreferencesFilePath;
  /*! \brief Path to the folder containing the configuration files.*/
  extern std::string kDaqConfigDirPath;
  /*! \brief Path to the last used configuration file.*/
  extern std::string kDefaultConfigFilePath;
  /*! \brief Path to the folder containing the GUI classes.*/
  extern std::string kDaqGuiDirPath;
  /*! \brief Path to the folder containing the necessary GUI pixmaps.*/
  extern std::string kDaqPixmapsDirPath;
  /*! \brief Path to the folder in which the data are written.*/
  extern std::string kDaqDataDirPath;
  /*! \brief Path to the folder in which the log files are written.*/
  extern std::string kDaqLogDirPath;
  /*! \brief Return the value of an environmental variable.*/
  extern std::string getEnvironmentVariable(std::string varName);
  /*! \brief Return the value of an environmental variable.*/
  extern std::string getEnvironmentVariable(std::string varName,
					    std::string defaultPath);
  /*! \brief Concatenate two paths inserting the correct separator.*/
  extern std::string join(std::string path1, std::string path2);
  /*! \brief Append a path (typically a file name) to
    {\ref xpolenv::kDaqRootDirPath}.*/
  extern std::string appendToDaqRoot(std::string path);
}

#endif
