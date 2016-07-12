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
#include "xServerGui.h"

xServerGui::xServerGui(QWidget *parent)
  : QDialog(parent)
{
  m_mainLayout = new QVBoxLayout;
  setupLabels();
  setupButtons();
  setLayout(m_mainLayout);
  setWindowTitle("xpol server");
  m_xServer = new xServer;
  setupConnections();
  readPreferences();
}

void xServerGui::setupLabels()
{
  m_inputFileLabel = new pQtCustomTextLabel(this, "Input file path");
  m_inputFileDisplay = new QLineEdit(this);
  m_inputFileLabel->setLabelStyle();
  m_mainLayout->addWidget(m_inputFileLabel);
  m_mainLayout->addWidget(m_inputFileDisplay);
  m_multicastAddressLabel = new pQtCustomTextLabel(this, "Multicast address");
  m_multicastAddressDisplay = new QLineEdit(this);
  m_multicastAddressLabel->setLabelStyle();
  m_mainLayout->addWidget(m_multicastAddressLabel);
  m_mainLayout->addWidget(m_multicastAddressDisplay);
  m_multicastPortLabel = new pQtCustomTextLabel(this, "Multicast port");
  m_multicastPortDisplay = new QLineEdit(this);
  m_multicastPortLabel->setLabelStyle();
  m_mainLayout->addWidget(m_multicastPortLabel);
  m_mainLayout->addWidget(m_multicastPortDisplay);
  m_sleepTimeLabel = new pQtCustomTextLabel(this, "Sleep time (ms)");
  m_sleepTimeDisplay = new QLineEdit(this);
  m_sleepTimeLabel->setLabelStyle();
  m_mainLayout->addWidget(m_sleepTimeLabel);
  m_mainLayout->addWidget(m_sleepTimeDisplay);
}

void xServerGui::setupButtons()
{
  m_buttonsLayout = new QHBoxLayout;
  m_startButton   = new QPushButton("Start");
  m_stopButton    = new QPushButton("Stop");
  m_quitButton    = new QPushButton("Quit");
  m_buttonsLayout->addWidget(m_startButton);
  m_buttonsLayout->addWidget(m_stopButton);
  m_buttonsLayout->addWidget(m_quitButton);
  m_mainLayout->addLayout(m_buttonsLayout);
  m_startButton->setEnabled(1);
  m_stopButton->setEnabled(0);
}

void xServerGui::setupConnections()
{
  connect(m_startButton, SIGNAL(clicked()), this, SLOT(start()));
  connect(m_stopButton, SIGNAL(clicked()), this, SLOT(stop()));
  connect(m_quitButton, SIGNAL(clicked()), this, SLOT(quit()));
  connect(m_xServer, SIGNAL(serverStopped()), this, SLOT(stop()));
}

std::string xServerGui::getInputFilePath()
{
  return m_inputFileDisplay->text().toStdString();
}

void xServerGui::setInputFilePath(std::string filePath)
{
  QString filePathQString = QString(filePath.c_str());
  m_inputFileDisplay->setText(filePathQString);
}

std::string xServerGui::getMulticastAddress()
{
  return m_multicastAddressDisplay->text().toStdString();
}

void xServerGui::setMulticastAddress(std::string address)
{
  QString addressQString = QString(address.c_str());
  m_multicastAddressDisplay->setText(addressQString);
}

int xServerGui::getMulticastPort()
{
  return m_multicastPortDisplay->text().toInt();
}

void xServerGui::setMulticastPort(int port)
{
  QString portQString = QString::number(port);
  m_multicastPortDisplay->setText(portQString);
}

int xServerGui::getSleepTime()
{
  return m_sleepTimeDisplay->text().toInt();
}

void xServerGui::setSleepTime(int msec)
{
  QString sleepQString = QString::number(msec);
  m_sleepTimeDisplay->setText(sleepQString);
}

void xServerGui::readPreferences()
{
  std::string cfgFolderPath = xpedaqos::rjoin("xpesrv");
  std::string preferencesFilePath = xpedaqos::join(cfgFolderPath,
						                          "preferences.cfg");
  *xpollog::kInfo << "Reading preferences from "
                  << preferencesFilePath << " ..." << endline;
  std::ifstream *inputFile =
    xpolio::kIOManager->openInputFile(preferencesFilePath);
  xpolio::kIOManager->skipLine(inputFile);
  setInputFilePath(xpolio::kIOManager->readLine(inputFile));
  xpolio::kIOManager->skipLine(inputFile);
  setMulticastAddress(xpolio::kIOManager->readLine(inputFile));
  xpolio::kIOManager->skipLine(inputFile);
  setMulticastPort(xpolio::kIOManager->readInteger(inputFile));
  xpolio::kIOManager->skipLine(inputFile);
  setSleepTime(xpolio::kIOManager->readInteger(inputFile));
  xpolio::kIOManager->closeInputFile(inputFile);
}

void xServerGui::writePreferences()
{
  std::string cfgFolderPath = xpedaqos::rjoin("xpesrv");
  std::string preferencesFilePath = xpedaqos::join(cfgFolderPath,
						                          "preferences.cfg");
  *xpollog::kInfo << "Writing preferences to " << preferencesFilePath
                  << " ..." << endline;
  std::ofstream *outputFile =
    xpolio::kIOManager->openOutputFile(preferencesFilePath);
  xpolio::kIOManager->write(outputFile, "//Input file path//");
  xpolio::kIOManager->write(outputFile, getInputFilePath());
  xpolio::kIOManager->write(outputFile, "//Multicast address//");
  xpolio::kIOManager->write(outputFile, getMulticastAddress());
  xpolio::kIOManager->write(outputFile, "//Multicast port//");
  xpolio::kIOManager->write(outputFile, getMulticastPort());
  xpolio::kIOManager->write(outputFile, "//Sleep time (ms)//");
  xpolio::kIOManager->write(outputFile, getSleepTime());
  xpolio::kIOManager->closeOutputFile(outputFile);
}

void xServerGui::start()
{
  m_startButton->setEnabled(0);
  m_stopButton->setEnabled(1);
  m_xServer->setInputFilePath(getInputFilePath());
  m_xServer->setMulticastAddress(getMulticastAddress());
  m_xServer->setMulticastPort(getMulticastPort());
  m_xServer->setSleepTime(getSleepTime());
  m_xServer->start();
}

void xServerGui::stop()
{
  m_startButton->setEnabled(1);
  m_stopButton->setEnabled(0);
  m_xServer->stop();
}

void xServerGui::quit()
{
  stop();
  writePreferences();
  close();
}
