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

#ifndef PUSERPREFERENCES_H
#define PUSERPREFERENCES_H

#include <iostream>

#include "xpedaqutils.h"
#include "xpollog.h"
#include "xpolio.h"


class pUserPreferences
{

 public:
  pUserPreferences() {;}
  pUserPreferences(std::string filePath);

  // Access methods.
  unsigned short int visualizationMode() const {return m_visualizationMode;}
  bool dataFileEnabled() const {return m_dataFileEnabled;}
  std::string outputFolder() const {return m_outputFolder;}
  bool multicastEnabled() const {return m_multicastEnabled;}
  std::string multicastAddress() const {return m_multicastAddress;}
  int multicastPort() const {return m_multicastPort;}
  bool logFileEnabled() const {return m_logFileEnabled;}
  int loggerTerminalLevel() const {return m_loggerTerminalLevel;}
  int loggerDisplayLevel() const {return m_loggerDisplayLevel;}
  int usbTimeout() const {return m_usbTimeout;}
  
  // Set methods.
  void setVisualizationMode(unsigned short int mode)
    {m_visualizationMode = mode;}
  void enableDataFile(bool enable) {m_dataFileEnabled = enable;}
  void setOutputFolder(std::string path) {m_outputFolder = path;}
  void enableMulticast(bool enable) {m_multicastEnabled = enable;}
  void setMulticastAddress(std::string address) {m_multicastAddress = address;}
  void setMulticastPort(int port) {m_multicastPort = port;}
  void enableLogFile(bool enable) {m_logFileEnabled = enable;}
  void setLoggerTerminalLevel(int level) {m_loggerTerminalLevel = level;}
  void setLoggerDisplayLevel(int level) {m_loggerDisplayLevel = level;}
  void setUsbTimeout(int timeout) {m_usbTimeout = timeout;}

  // Read/write to/from file.
  void writeToFile(std::string filePath);
  void readFromFile(std::string filePath);

  // Terminal formatting.
  std::ostream& fillStream(std::ostream& os) const;
  friend std::ostream& operator<<(std::ostream& os,
				  const pUserPreferences& preferences)
  {return preferences.fillStream(os);}
  
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
  int m_usbTimeout;
};

#endif
