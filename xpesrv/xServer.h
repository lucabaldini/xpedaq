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

#ifndef XSERVER_H
#define XSERVER_H

#include <QThread>
#include <fstream>  //added by Gloria
#include "pUdpSender.h"

class xServer : public QThread
{
  Q_OBJECT

 public:
  xServer(QWidget *parent = 0);
  void setInputFilePath(std::string filePath);
  void setMulticastAddress(std::string address);
  void setMulticastPort(int port);
  void setSleepTime(int msec);
  void broadcastEvent();
  bool isRunning();

 public slots:
  void stop();

 signals:
  void serverStopped();
  void eventBroadcasted(int numEvents);
 
 protected: 
  void run();

 private:
  pUdpSender *m_udpSender;
  std::string m_inputFilePath;
  FILE *m_inputFile;
  std::string m_multicastAddress;
  int m_multicastPort;
  int m_sleepTime;
  bool m_running;
  int m_numBroadcastedEvents;
  std::ifstream fStream; //added by Gloria
  int fErr;         // Error code. added by Gloria
};

#endif
