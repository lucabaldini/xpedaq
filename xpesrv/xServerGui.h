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

#ifndef XSERVERGUI_H
#define XSERVERGUI_H

#include <QDialog>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLineEdit>

#include "pQtCustomTextLabel.h"
#include "xServer.h"
#include "xpolio.h"
#include "xpedaqos.h"

class xServerGui : public QDialog
{
  Q_OBJECT

 public:
  xServerGui(QWidget *parent = 0);

 private slots:
  void start();
  void stop(); 
  void quit();

 private:
  void setupLabels();
  void setupButtons();
  void setupConnections();
  std::string getInputFilePath();
  void setInputFilePath(std::string filePath);
  std::string getMulticastAddress();
  void setMulticastAddress(std::string address);
  int getMulticastPort();
  void setMulticastPort(int port);
  int getSleepTime();
  void setSleepTime(int msec);
  void readPreferences();
  void writePreferences();
  pQtCustomTextLabel *m_inputFileLabel;
  QLineEdit *m_inputFileDisplay;
  pQtCustomTextLabel *m_multicastAddressLabel;
  QLineEdit *m_multicastAddressDisplay;
  pQtCustomTextLabel *m_multicastPortLabel;
  QLineEdit *m_multicastPortDisplay;
  pQtCustomTextLabel *m_sleepTimeLabel;
  QLineEdit *m_sleepTimeDisplay;
  QPushButton *m_startButton;
  QPushButton *m_stopButton;
  QPushButton *m_quitButton;
  QHBoxLayout *m_buttonsLayout;
  QVBoxLayout *m_mainLayout;
  xServer     *m_xServer;
};

#endif
