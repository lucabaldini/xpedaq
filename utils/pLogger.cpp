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


#include "pLogger.h"


pLogger::pLogger(int terminalLevel, int displayLevel)
{
  m_debugChannel = new pLoggerChannel("DEBUG");  
  m_infoChannel = new pLoggerChannel("INFO ");  
  m_warningChannel = new pLoggerChannel("WARN ");  
  m_errorChannel = new pLoggerChannel("ERROR");
  setTerminalLevel(terminalLevel);
  setDisplayLevel(displayLevel);
  enableLogFile(false);
}

void pLogger::setLogFilePath(std::string filePath)
{
  m_debugChannel->setLogFilePath(filePath);
  m_infoChannel->setLogFilePath(filePath);
  m_warningChannel->setLogFilePath(filePath);
  m_errorChannel->setLogFilePath(filePath);
}

void pLogger::setTerminalLevel(int level)
{
  m_debugChannel->enableTerminal(level <= Debug);
  m_infoChannel->enableTerminal(level <= Info);
  m_warningChannel->enableTerminal(level <= Warning);
  m_errorChannel->enableTerminal(level <= Error);
}

void pLogger::setDisplayLevel(int level)
{
  m_debugChannel->enableDisplay(level <= Debug);
  m_infoChannel->enableDisplay(level <= Info);
  m_warningChannel->enableDisplay(level <= Warning);
  m_errorChannel->enableDisplay(level <= Error);
}

void pLogger::enableLogFile(bool enable)
{
  std::cout << "Enable file " << enable << std::endl;
  m_debugChannel->enableLogFile(enable);
  m_infoChannel->enableLogFile(enable);
  m_warningChannel->enableLogFile(enable);
  m_errorChannel->enableLogFile(enable);
  if (!enable) {
    setLogFilePath("NULL");
  }
}
