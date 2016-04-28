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

#include "pUdpSender.h"


pUdpSender::pUdpSender()
{
  m_udpSocket = new QUdpSocket();
  setHostAddress("127.0.0.1");
  setPort(0);
}

void pUdpSender::setHostAddress(std::string address)
{
  m_hostAddress = QHostAddress(address.c_str());
}

void pUdpSender::setPort(int port)
{
  m_port = port;
}

void pUdpSender::broadcastDatagram(pDataBlock *block)
{
  write(block->getCharDataBlock(), block->size());
}

void pUdpSender::broadcastEventByEvent(pDataBlock *block)
{  
  std::vector<unsigned int> offsetVec = block->offsetVec();
  std::vector<unsigned int>::iterator offset;
  for(offset = offsetVec.begin(); offset != offsetVec.end() - 1; offset++) {
    write(block->getCharDataBlock(*offset), *(offset + 1) - *offset);
  }
}

void pUdpSender::broadcastDatagram(char *datagram, int size)
{
  write(datagram, size);
}

void pUdpSender::broadcastDatagram(unsigned char *datagram, int size)
{
  write((char*)datagram, size);
}

void pUdpSender::write(char *data, int size)
{
  int numBytes = m_udpSocket->writeDatagram(data, size, m_hostAddress, m_port);
  if (numBytes < 0) {
    int errorCode = m_udpSocket->error();
    *xpollog::kDebug << "UDP socket error. " << numBytes << "/" << size
		     << " bytes multicasted (error code " << errorCode
		     << ")." << endline;
  }
}
