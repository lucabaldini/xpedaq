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

#ifndef PUSERPREFERENCES_H
#define PUSERPREFERENCES_H

#include <iostream>

#include "xpollog.h"
#include "xpolio.h"


class pUserPreferences
{

 public:
  pUserPreferences();
  pUserPreferences(std::string filePath);
  ~pUserPreferences();
  inline unsigned short int getVisualizationMode() const
    {return m_visualizationMode;}
  inline void setVisualizationMode(unsigned short int mode)
    {m_visualizationMode = mode;}
  inline bool dataFileEnabled() const
    {return m_dataFileEnabled;}
  inline void enableDataFile(bool enable)
    {m_dataFileEnabled = enable;}
  inline std::string outputFolder() const
    {return m_outputFolder;}
  inline void setOutputFolder(std::string path)
    {m_outputFolder = path;}
  inline bool multicastEnabled() const
    {return m_multicastEnabled;}
  inline void enableMulticast(bool enable)
    {m_multicastEnabled = enable;}
  inline std::string getMulticastAddress() const
    {return m_multicastAddress;}
  inline void setMulticastAddress(std::string address)
    {m_multicastAddress = address;}
  inline int getMulticastPort() const
    {return m_multicastPort;}
  inline void setMulticastPort(int port)
    {m_multicastPort = port;}
  inline bool logFileEnabled() const
    {return m_logFileEnabled;}
  inline void enableLogFile(bool enable)
    {m_logFileEnabled = enable;}
  inline int getLoggerTerminalLevel() const
    {return m_loggerTerminalLevel;}
  inline void setLoggerTerminalLevel(int level)
    {m_loggerTerminalLevel = level;}
  inline int getLoggerDisplayLevel() const
    {return m_loggerDisplayLevel;}
  inline void setLoggerDisplayLevel(int level)
    {m_loggerDisplayLevel = level;}
  void writeToFile(std::string filePath);
  void readFromFile(std::string filePath);  
  
 protected:

 private:
  unsigned short int m_visualizationMode;
  bool m_dataFileEnabled;
  std::string m_outputFolder;
  bool m_multicastEnabled;
  std::string m_multicastAddress;
  int m_multicastPort;
  bool m_logFileEnabled;
  int m_loggerTerminalLevel;
  int m_loggerDisplayLevel;

};

#endif
