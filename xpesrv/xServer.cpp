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

#include <QtGui>
#include <QtNetwork>

#include "xServer.h"

#define COMB(x1,x2) ((((x2 & 0xff)<<8)|x1) & 0xff)

xServer::xServer(QWidget *parent)
{
  m_running = false;
}

bool xServer::isRunning()
{
  return m_running;
}

void xServer::setInputFilePath(std::string filePath)
{
  m_inputFilePath = filePath;
}

void xServer::setMulticastAddress(std::string address)
{
  m_multicastAddress = address;
}

void xServer::setMulticastPort(int port)
{
  m_multicastPort = port;
}

void xServer::setSleepTime(int msec)
{
  if (msec >= 0)
    {
      m_sleepTime = msec;
    }
  else
    {
      m_sleepTime = 0;
    }
}

void xServer::run()
{
  *xpollog::kDebug << "Starting the xpol server..." << endline;
  *xpollog::kDebug << "Input file: " << m_inputFilePath << endline;
  *xpollog::kDebug << "Address   : " << m_multicastAddress << endline;
  *xpollog::kDebug << "Port      : " << m_multicastPort << endline;
  //if (!(m_inputFile = fopen(m_inputFilePath.c_str(), "r")))
  fStream.open(m_inputFilePath.c_str(), std::ios::in | std::ios::binary );
  fErr = fStream.bad();
  if ( fErr ) 
    {
      *xpollog::kError << "Could not open input file. Server start failed" <<
	endline;
      emit serverStopped();
    }
  else{
    m_udpSender = new pUdpSender();
    m_udpSender->setHostAddress(m_multicastAddress);
    m_udpSender->setPort(m_multicastPort);
    m_numBroadcastedEvents = 0;
    m_running = true;
    while (m_running)
      {
	broadcastEvent();
	m_numBroadcastedEvents ++;
	emit eventBroadcasted(m_numBroadcastedEvents);
	msleep(m_sleepTime);
      }
  }
}

void xServer::stop()
{
  if (m_running)
    {
      m_running = false;
      *xpollog::kDebug << "Stopping the xpol server..." << endline;
      *xpollog::kInfo << m_numBroadcastedEvents << " events broadcasted." <<
	endline;
      //fclose(m_inputFile);
      fStream.close();
    }
}


bool xServer::isRoiValid(const int* Roi)
{
  return ((Roi[0] >= 0) && (Roi[1] < 300) && (Roi[1] > Roi [0])
           && (Roi[2] >= 0) && (Roi[3] < 352) && (Roi[3] > Roi[2]));
}


void xServer::goToNextEvent()
{
  *xpollog::kInfo << "Looking for a good header..." << endline;
  // find next event 
  char tmp1, tmp2;
  int tag =0;
  int counter =0;
  do{
    fStream.read((char*)&tmp1, sizeof(char));
    fStream.read((char*)&tmp2, sizeof(char));
    tag = COMB(tmp1,tmp2);
    counter+=2;
  } while ((tag != 65535) && (!fStream.eof()));
  if (fStream.eof()){
    *xpollog::kInfo << "EOF reached without finding other good events. "
                    << "( " << counter << " characters read)."
                    << endline;
    stop();
    emit serverStopped();
    return;
  }
  *xpollog::kInfo << "Good event found" << endline;
  // go back two characters, so that the next iteration finds the cursor in
  // the expected poisition (i.e. at the beginning of the header)
  fStream.putback(tmp2);
  fStream.putback(tmp1);
}



void xServer::broadcastEvent()
{
  const int maxEvtsize = 200000; 
  //unsigned char data[2];
  unsigned char buffer[maxEvtsize];
  char tmp1, tmp2;
  int bindex = 0;
  //fread(data, 1, 2, m_inputFile);
  //int tag = COMB(data[0],data[1]);
  fStream.read((char*)&tmp1, sizeof(char));
  fStream.read((char*)&tmp2, sizeof(char));
  int tag = COMB(tmp1,tmp2);
  if(tag != 65535)
    {
      *xpollog::kInfo << "Tag " << tag << " found." << endline;
      goToNextEvent();
      return;
    }
  buffer[bindex]  = tmp1; //data[0];
  buffer[bindex+1]= tmp2; //data[1];
  bindex+=2;
  // read Xmin, Xmax, Ymin, Ymax and get number of pixel to read
  int Roi[4];
  for(int j=0; j<4; j++){
    //fread(data,1,2,m_inputFile);
    //Roi[j] = COMB(data[0],data[1]);
    fStream.read((char*)&tmp1, sizeof(char));
    fStream.read((char*)&tmp2, sizeof(char));
    Roi[j] = COMB(tmp1,tmp2);
    buffer[bindex]  = tmp1; //data[0];
    buffer[bindex+1]= tmp2; //data[1];         
    bindex+=2;
  }
   *xpollog::kDebug << Roi[0] << " " << Roi[1] << " " << Roi[2] << " "
                  << Roi[3] << endline;
  int numPix = (Roi[1]+1- Roi[0])*(Roi[3]+1- Roi[2]);
  *xpollog::kDebug << numPix << endline;
  // read BufferID and Time Stamps
  //fread(data,1,2,m_inputFile);
  fStream.read((char*)&tmp1, sizeof(char));
  fStream.read((char*)&tmp2, sizeof(char));
  buffer[bindex]  = tmp1; //data[0];
  buffer[bindex+1]= tmp2; //data[1];
  bindex+=2;
  
  //fread(data,1,2,m_inputFile);    
  fStream.read((char*)&tmp1, sizeof(char));
  fStream.read((char*)&tmp2, sizeof(char));
  buffer[bindex]  = tmp1; //data[0];
  buffer[bindex+1]= tmp2; //data[1];
  bindex+=2;

  //fread(data,1,2,m_inputFile);
  fStream.read((char*)&tmp1, sizeof(char));
  fStream.read((char*)&tmp2, sizeof(char));
  buffer[bindex]  =  tmp1; //data[0];
  buffer[bindex+1]=  tmp2; //data[1];
  bindex+=2;

  //fread(data,1,2,m_inputFile);
  fStream.read((char*)&tmp1, sizeof(char));
  fStream.read((char*)&tmp2, sizeof(char));
  buffer[bindex]  = tmp1; //data[0];
  buffer[bindex+1]= tmp2; //data[1];
  bindex+=2;

  //fread(data,1,2,m_inputFile);
  fStream.read((char*)&tmp1, sizeof(char));
  fStream.read((char*)&tmp2, sizeof(char));
  buffer[bindex]  = tmp1; //data[0];
  buffer[bindex+1]= tmp2; //data[1];
  bindex+=2;

  if (!isRoiValid(Roi)){
    *xpollog::kError << "Invalid ROI. Skipping event..." << endline;
    goToNextEvent();
    return;
  }
  if ((2*numPix) > maxEvtsize){
    *xpollog::kError << "Evt too big (" << numPix << " pixels)."
                     << endline;
    fStream.ignore(2*numPix);
    return;
  }
  for(int j=0; j<numPix ; j++)
    {
      //fread(data,1,2,m_inputFile);
      fStream.read((char*)&tmp1, sizeof(char));
      fStream.read((char*)&tmp2, sizeof(char));
      buffer[bindex]  = tmp1; //data[0];
      buffer[bindex+1]= tmp2; //data[1];
      bindex+=2;
    }
  *xpollog::kError << "Broadcasting evt" << endline;
  m_udpSender->broadcastDatagram((char*)buffer, bindex);//maxEvsize);
}
