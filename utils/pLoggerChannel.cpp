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

#include "pLoggerChannel.h"
#include "pChrono.h"


pLoggerChannel::pLoggerChannel(std::string name)
  : m_name(name),
    m_logFilePath("NULL"),
    m_lineTerminated(true),
    m_terminalEnabled(false),
    m_displayEnabled(false),
    m_logFileEnabled(false)
{
  m_baseString = new QString("");
  m_headerString = new QString(("[" + m_name + "] ").c_str());
  m_messageBuffer = new QTextStream(m_baseString);
}

void pLoggerChannel::reset()
{
  m_baseString->truncate(0);
}

void pLoggerChannel::write(const QString &s)
{
  if (m_terminalEnabled) {
    std::cout << s.toStdString();
  }
  if (m_displayEnabled) {
    emit message(s);
  }
  if (m_logFileEnabled) {
    std::ofstream logFile(m_logFilePath.c_str(),
			  std::ios_base::out | std::ios_base::app);
    logFile << s.toStdString();
    // This is horrible. We should have a better way to figure out whether
    // we need to write the timestamp.
    if (m_lineTerminated) {
      logFile << "(" << pChrono::datetime(true) << ") ";
    }
    logFile.close();
  }
  m_lineTerminated = (s.right(1) == "\n");
}


/*! Note that write() is called twice, here---once for the header and once
for the message.
 */
void pLoggerChannel::flush()
{
  if (m_lineTerminated) {
    write(*m_headerString);
  }
  write(*m_baseString);
  reset();
}

void pLoggerChannel::hex()
{
  m_messageBuffer->setIntegerBase(16);
}

void pLoggerChannel::dec()
{
  m_messageBuffer->setIntegerBase(10);
}

pLoggerChannel &pLoggerChannel::operator<<(bool b)
{
  *m_messageBuffer << b;
  return *this;
}

pLoggerChannel &pLoggerChannel::operator<<(char ch)
{
  *m_messageBuffer << ch;
  return *this;
}

pLoggerChannel &pLoggerChannel::operator<<(signed short i)
{
  *m_messageBuffer << i;
  return *this;
}

pLoggerChannel &pLoggerChannel::operator<<(unsigned short i)
{
  *m_messageBuffer << i;
  return *this;
}

pLoggerChannel &pLoggerChannel::operator<<(signed int i)
{
  *m_messageBuffer << i;
  return *this;
}

pLoggerChannel &pLoggerChannel::operator<<(unsigned int i)
{
  *m_messageBuffer << i;
  return *this;
}

pLoggerChannel &pLoggerChannel::operator<<(signed long i)
{
  *m_messageBuffer << i;
  return *this;
}

pLoggerChannel &pLoggerChannel::operator<<(unsigned long i)
{
  *m_messageBuffer << i;
  return *this;
}

pLoggerChannel &pLoggerChannel::operator<<(float f)
{
  *m_messageBuffer << f;
  return *this;
}

pLoggerChannel &pLoggerChannel::operator<<(double f)
{
  *m_messageBuffer << f;
  return *this;
}

pLoggerChannel &pLoggerChannel::operator<<(const QString &s)
{
  *m_messageBuffer << s;
  return *this;
}

pLoggerChannel &pLoggerChannel::operator<<(const std::string &s)
{
  *m_messageBuffer << s.c_str();
  return *this;
}

pLoggerChannel &pLoggerChannel::operator<<(const char *c)
{
  *m_messageBuffer << c;
  return *this;
}

pLoggerChannel &flush(pLoggerChannel &channel)
{
  channel.flush();
  return channel;
}

pLoggerChannel &endline(pLoggerChannel &channel)
{
  channel << "\n" << flush;
  return channel;
}

pLoggerChannel &hex(pLoggerChannel &channel)
{
  channel.hex();
  return channel;
}

pLoggerChannel &dec(pLoggerChannel &channel)
{
  channel.dec();
  return channel;
}

