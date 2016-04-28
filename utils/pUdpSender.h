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

#ifndef PUDPSENDER_H
#define PUDPSENDER_H

#include <QObject>
#include <QUdpSocket>
#include <QHostAddress>

#include "xpollog.h"
#include "pDataBlock.h"

class pUdpSender : public QObject
{
  Q_OBJECT

 public:

  pUdpSender();
  ~pUdpSender() {;}
  void setHostAddress(std::string address);
  void setPort(int port);
  void write(char *data, int size);

 public slots:

  void broadcastEventByEvent(pDataBlock *block);
  void broadcastDatagram(pDataBlock *block);
  void broadcastDatagram(char *datagram, int size);
  void broadcastDatagram(unsigned char *datagram, int size);

 private:

  QUdpSocket *m_udpSocket;
  QHostAddress m_hostAddress;
  int m_port;

};
#endif
