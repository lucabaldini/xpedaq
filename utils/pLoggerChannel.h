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

#ifndef PLOGGERCHANNEL_H
#define PLOGGERCHANNEL_H

#include <QTextStream>
#include <QIODevice>
#include <QString>
#include <iostream>
#include <fstream>
#include <ctime>


class pLoggerChannel : public QObject
{
  Q_OBJECT

 public:
  pLoggerChannel(std::string name);
  ~pLoggerChannel() {;}
  inline std::string name() const {return m_name;}
  inline bool lineTerminated() const {return m_lineTerminated;}
  inline void enableTerminal(bool enabled) {m_terminalEnabled = enabled;}
  inline void enableDisplay(bool enabled) {m_displayEnabled = enabled;}
  inline void enableLogFile(bool enabled) {m_logFileEnabled = enabled;}
  inline void setLogFilePath(std::string filePath) {m_logFilePath = filePath;}
  pLoggerChannel &operator<<(bool b);
  pLoggerChannel &operator<<(char ch);
  pLoggerChannel &operator<<(signed short i);
  pLoggerChannel &operator<<(unsigned short i);
  pLoggerChannel &operator<<(signed int i);
  pLoggerChannel &operator<<(unsigned int i);
  pLoggerChannel &operator<<(signed long i);
  pLoggerChannel &operator<<(unsigned long i);
  pLoggerChannel &operator<<(float f);
  pLoggerChannel &operator<<(double f);
  pLoggerChannel &operator<<(const QString &s);
  pLoggerChannel &operator<<(const std::string &s);
  pLoggerChannel &operator<<(const char *c);
  void flush();
  void dec();
  void hex();

 private:
  std::string m_name;
  std::string m_logFilePath;
  bool m_lineTerminated;
  QString *m_baseString;
  QString *m_headerString;
  QTextStream *m_messageBuffer;
  bool m_terminalEnabled;
  bool m_displayEnabled;
  bool m_logFileEnabled;
  void reset();
  void write(const QString &s);
  std::string timestamp();

 signals:
  void message(QString message);
};

typedef pLoggerChannel &(*pLoggerChannelFunction)(pLoggerChannel &);
inline pLoggerChannel &operator<<(pLoggerChannel &channel,
				  pLoggerChannelFunction function)
{return (*function)(channel); }
pLoggerChannel &flush(pLoggerChannel &channel);
pLoggerChannel &endline(pLoggerChannel &channel);
pLoggerChannel &hex(pLoggerChannel &channel);
pLoggerChannel &dec(pLoggerChannel &channel);

#endif //PLOGGERCHANNEL_H
