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

#ifndef PLOGGER_H
#define PLOGGER_H

#include <iostream>

#include "pLoggerChannel.h"
#include "xpolenv.h"

class pLogger
{
  
 public:

  pLogger(int terminalLevel = Debug, int displayLevel = Debug);
  ~pLogger() {;}  
  inline pLoggerChannel *debug() const {return m_debugChannel;}
  inline pLoggerChannel *info() const {return m_infoChannel;}
  inline pLoggerChannel *warning() const {return m_warningChannel;}
  inline pLoggerChannel *error() const {return m_errorChannel;}
  void setTerminalLevel(int level);
  void setDisplayLevel(int level);
  void enableLogFile(bool enable);
  void setLogFilePath(std::string filePath);

  enum LoggerLevel{
    Debug,
    Info,
    Warning,
    Error
  };

 private:
  
  pLoggerChannel *m_debugChannel;
  pLoggerChannel *m_infoChannel;
  pLoggerChannel *m_warningChannel;
  pLoggerChannel *m_errorChannel;
  std::string m_logFilePath;
};

#endif //PLOGGER_H
