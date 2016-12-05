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

#include <QFileDialog>

#include "xpedaqos.h"
#include "pedviewerWindow.h"
#include "xpollog.h"

/*
*/
pedviewerWindow::pedviewerWindow(PedFile* inputFile,
                                 PedmapFile* referenceFile,
                                 QWidget *parent, int posx, int posy,
                                 int windowHeight, int windowWidth) :
  QMainWindow(parent),
  m_posx (posx),
  m_posy(posy),
  m_windowHeight(windowHeight),
  m_windowWidth(windowWidth),
  m_inputFile(inputFile),
  m_referenceFile(referenceFile),
  m_curEvent(-1)
{
  if (objectName().isEmpty())
    setObjectName(QString::fromUtf8("DisplayWindow"));
  resize(m_windowHeight, m_windowWidth);
  m_centralWidget = new QWidget(this);
  m_centralWidget -> setObjectName(QString::fromUtf8("centralWidget"));
  m_verticalLayout = new QVBoxLayout(m_centralWidget);
  m_verticalLayout -> setSpacing(6);
  m_verticalLayout -> setContentsMargins(11, 11, 11, 11);
  m_verticalLayout -> setObjectName(QString::fromUtf8("verticalLayout"));

  m_plotGrid = new pedviewerPlotGrid();
  m_verticalLayout -> addWidget(m_plotGrid);

  setCentralWidget(m_centralWidget);
  setGeometry(m_posx, m_posy, m_windowHeight, m_windowWidth);

  //Read the pedestal map from file
  m_nEvents = m_inputFile->nEvents();
  m_plotGrid->setTotEvtLabel(m_nEvents);
  m_pedMap = new PedestalsMap();
  m_plotGrid->setButtonsEnabled(false);
  
  if (m_inputFile->fileType() == PedFile::dataType){
    int evt = readFirstEventToDisplay();
    m_curEvent = m_inputFile->fillPedMap(*m_pedMap, 1, evt);
    updateNavBarStatus(m_curEvent);
  } else {
    m_curEvent = m_inputFile->fillPedMap(*m_pedMap);
  }
  
  if (!(m_referenceFile == nullptr)){
    m_referenceMap = new PedestalsMap();
    m_referenceFile->fillPedMap(*m_referenceMap);
  } else {
    m_referenceMap = nullptr;
  }
  updatePlots();
  
  connect(m_plotGrid->m_nextButton, SIGNAL(clicked()), 
          this, SLOT(nextPressed()));
  connect(m_plotGrid->m_prevButton, SIGNAL(clicked()), 
          this, SLOT(prevPressed()));
}


/* Display the results (average and RMS) of a pedestal measurement
 */
void pedviewerWindow::showPedestals()
{  
  m_plotGrid -> fillPlots(*m_pedMap);
  m_plotGrid -> replotAll();
}


/* Display the results (average and RMS) of a pedestal measurement
   subtracting a reference map
 */
void pedviewerWindow::showPedestalsWithRef()
{  
  m_plotGrid -> fillPlots(*m_pedMap, *m_referenceMap);
  m_plotGrid -> replotAll();
}


/*
*/
void pedviewerWindow::nextPressed()
{ 
  *xpollog::kInfo << "Next" << endline;
  m_plotGrid->clear();
  m_pedMap->reset();
  m_curEvent = m_inputFile->fillPedMap(*m_pedMap, 1, m_curEvent+1);
  updateNavBarStatus(m_curEvent);
  updatePlots();
}


/*
*/
void pedviewerWindow::prevPressed()
{
  *xpollog::kInfo << "Prev" << endline;
  m_plotGrid->clear();
  m_pedMap->reset();
  m_curEvent = m_inputFile->fillPedMap(*m_pedMap, 1, m_curEvent-1);
  updateNavBarStatus(m_curEvent);
  updatePlots();
}


/*
*/
void pedviewerWindow::updatePlots()
{
  if (!m_referenceMap){
    showPedestals();
  } else {   
    showPedestalsWithRef();
  }
}

/*
*/
void pedviewerWindow::updateNavBarStatus(int curEvent)
{
  m_plotGrid->updateEvtNumberEdit(curEvent);
  m_plotGrid->setButtonsEnabled(true);
  if (curEvent == 0) m_plotGrid->setPrevButtonEnabled(false);
  if (curEvent == m_nEvents-1) m_plotGrid->setNextButtonEnabled(false);
}


/*
*/
void pedviewerWindow::closeEvent(QCloseEvent *event)
{
  emit windowClosed();
  event->accept();
}


/*
*/
int pedviewerWindow::readNumEventsToDisplay()
{
  int nEvents=1;
  std::cout << "Select the number of events to be read "
            << "[default = 1]: ";
  std::string strInput = "";
  while (true){
    getline(std::cin, strInput);
    // This code converts from string to number safely.
    std::stringstream inputStream(strInput);
    if (inputStream.str() == "" || (inputStream >> nEvents)){
      break;
    }
    std::cout << "Invalid input, please try again" << std::endl;
  }
  return nEvents;
}


/*
*/
int pedviewerWindow::readFirstEventToDisplay()
{
  int firstEvent=0;
  *xpollog::kInfo << "Reading full frame events... " << endline;    
  std::cout << "Select the first event to be read [default = 0 "
               << "(first event)]: ";
  std::string strInput = "";
  while (true){
    getline(std::cin, strInput);
    // This code converts from string to number safely.
    std::stringstream inputStream(strInput);
    if (inputStream.str() == "" || (inputStream >> firstEvent)){
      break;
    }
    std::cout << "Invalid input, please try again" << std::endl;
  }
  return firstEvent;
}

