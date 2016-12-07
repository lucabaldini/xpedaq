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

#include "pedviewerWindow.h"
#include "xpollog.h"

/*
*/
pedviewerWindow::pedviewerWindow(QWidget *parent, int windowHeight,
                                 int windowWidth) :
  QMainWindow(parent),
  m_windowHeight(windowHeight),
  m_windowWidth(windowWidth),
  m_inputFile(nullptr),
  m_pedMap(nullptr),
  m_referenceMap(nullptr),
  m_curEvent(-1)
{
  if (objectName().isEmpty())
    setObjectName(QString::fromUtf8("DisplayWindow"));
  resize(m_windowHeight, m_windowWidth);
  m_centralWidget = new QWidget(this);
  m_centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
  setCentralWidget(m_centralWidget);
  
  m_verticalLayout = new QVBoxLayout(m_centralWidget);
  m_verticalLayout->setContentsMargins(10, 10, 10, 10);
  m_verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));

  m_menuBar = new PedviewerMenuBar();
  m_verticalLayout->setMenuBar(m_menuBar);
  setupNavBar();
  setNavBarEnabled(false);

  m_plotGrid = new pedviewerPlotGrid(this);
  m_verticalLayout->addWidget(m_plotGrid, 1);

  m_pedMap = new PedestalsMap();
  setupConnections();
}


/*
*/
void pedviewerWindow::setupNavBar()
{
  m_navLayout = new QHBoxLayout();
  m_prevButton = new QPushButton("<");
  m_prevButton->setFixedWidth(60);
  m_nextButton = new QPushButton(">");
  m_nextButton->setFixedWidth(60);
  m_evtNumberEdit = new pQtCustomLineEdit<int>();
  m_evtNumberEdit->setFixedWidth(60);
  QString initialLabel = QString("/ ") + QString::number(0);
  m_totEvtLabel = new pQtCustomTextLabel(this, initialLabel);
  m_totEvtLabel->setFixedWidth(50);
  m_subtractRefCheckBox = new QCheckBox("Subtract reference");
  m_navLayout->addWidget(m_prevButton);
  m_navLayout->addWidget(m_nextButton);
  m_navLayout->addWidget(m_evtNumberEdit);
  m_navLayout->addWidget(m_totEvtLabel);
  m_navLayout->addWidget(m_subtractRefCheckBox);
  m_verticalLayout->addLayout(m_navLayout, 0);
}


/*
*/
void pedviewerWindow::setButtonsEnabled(bool enabled)
{
  m_prevButton->setEnabled(enabled);
  m_nextButton->setEnabled(enabled);
}


/*
*/
void pedviewerWindow::setNavBarEnabled(bool enabled)
{
  m_evtNumberEdit->setEnabled(enabled);
  setButtonsEnabled(enabled);
  m_subtractRefCheckBox->setEnabled(enabled);
}


/*
*/
void pedviewerWindow::setTotEvtLabel(int numEvents)
{
  QString label = QString("/ ") + QString::number(numEvents);
  m_totEvtLabel->setText(label);
}


/*
*/
void pedviewerWindow::updateEvtNumberEdit(int curEvent)
{
  m_evtNumberEdit->setVal(curEvent);
}


/*
*/
void pedviewerWindow::setEvtNumberEditRange(int min, int max)
{
  m_evtNumberEdit->setRange(min, max);
}


/*
*/
void pedviewerWindow::openDataFile(const QString& filePath)
{
  if(filePath.isEmpty()){
    return;
  }
  //Convert QString to std::string
  std::string filePathString = filePath.toUtf8().constData();
  // Determine the file type
  PedFile::inputFileType fileType = identifyFileType(filePathString);
  // If the type is unknown do nothing
  if (fileType == PedFile::unknownType){
    *xpollog::kError << "Unknown file type: " << filePathString
                     << "Exiting..." << endline;
    return;
  }
  // Delete the current file (by C++ specifics this operation is harmless,
  // even if there is no file opened)
  delete m_inputFile;
  // Open the file with the appropriate type
  m_inputFile = new PedDataFile(filePathString);
  // Read number of events in the file
  m_nEvents = m_inputFile->nEvents();
  // Setup navigation bar
  setTotEvtLabel(m_nEvents);
  setEvtNumberEditRange(1, m_nEvents);
  // Plot the first event
  showEvent(1);
}


/*
*/
void pedviewerWindow::openMapFile(const QString& filePath)
{
  if(filePath.isEmpty()){
    return;
  }
  //Convert QString to std::string
  std::string filePathString = filePath.toUtf8().constData();
  // Determine the file type
  PedFile::inputFileType fileType = identifyFileType(filePathString);
  // If the type is unknown do nothing
  if (fileType == PedFile::unknownType){
    *xpollog::kError << "Unknown file type: " << filePathString
                     << "Exiting..." << endline;
    return;
  }
  // Delete the current file (by C++ specifics this operation is harmless,
  // even if there is no file opened)
  delete m_inputFile;
  // Open the file with the appropriate type
  m_inputFile = new PedmapFile(filePathString);
  //Read number of events from the file
  m_nEvents = m_inputFile->nEvents();
  // Setup navigation bar
  setNavBarEnabled(false);
  setTotEvtLabel(m_nEvents);
  updateNavBarStatus(m_nEvents);
  // Plot the map
  showMap();
}


/*
*/
void pedviewerWindow::loadReferenceFile(const QString& filePath)
{
  if(filePath.isEmpty()){
    return;
  }
  // Convert QString to std::string
  std::string filePathString = filePath.toUtf8().constData();
  // Open the file
  PedmapFile* referenceFile = new PedmapFile(filePathString);
  // Read data from the file
  if (m_referenceMap == nullptr){
    m_referenceMap = new PedestalsMap();
  } else {
    m_referenceMap->reset();
  }
  referenceFile->fillPedMap(*m_referenceMap);
  // Close te file and release the memory
  delete referenceFile;
  if (!(m_inputFile==nullptr)){
    m_subtractRefCheckBox->setEnabled(true);
    updatePlots();
  }
}


/* Display the results (average and RMS) of a pedestal measurement
 */
void pedviewerWindow::showPedestals()
{  
  m_plotGrid->clear();
  m_plotGrid->fillPlots(*m_pedMap);
  m_plotGrid->replotAll();
}


/* Display the results (average and RMS) of a pedestal measurement
   subtracting a reference map
 */
void pedviewerWindow::showPedestalsWithRef()
{  
  m_plotGrid->clear();
  m_plotGrid->fillPlots(*m_pedMap, *m_referenceMap);
  m_plotGrid->replotAll();
}


/*
*/
void pedviewerWindow::showEvent(int evtNumber)
{ 
  m_pedMap->reset();
  m_curEvent = m_inputFile->fillPedMap(*m_pedMap, evtNumber);
  updatePlots();
  setNavBarEnabled(true);  
  updateNavBarStatus(m_curEvent);  
}


/*
*/
void pedviewerWindow::showMap()
{
  m_pedMap->reset();
  m_curEvent = m_inputFile->fillPedMap(*m_pedMap);
  updatePlots();
}


/*
*/
void pedviewerWindow::nextPressed()
{ 
  showEvent(m_curEvent+1);
}


/*
*/
void pedviewerWindow::prevPressed()
{
  showEvent(m_curEvent-1);
}


/*
*/
void pedviewerWindow::evtNumberEditChanged()
{
  int evtRequested = m_evtNumberEdit->value();
  if (evtRequested != m_curEvent){
    showEvent(m_evtNumberEdit->value());
  }
}


/*
*/
void pedviewerWindow::updatePlots()
{
  if (m_subtractRefCheckBox->isChecked()){
    showPedestalsWithRef();
  } else {
    showPedestals();
  }
}

/*
*/
void pedviewerWindow::updateNavBarStatus(int curEvent)
{
  if (curEvent == 1) m_prevButton->setEnabled(false);
  if (curEvent == m_nEvents) m_nextButton->setEnabled(false);
  updateEvtNumberEdit(curEvent);
  // Activate the checkbox only if there is a reference map loaded
  m_subtractRefCheckBox->setEnabled(!(m_referenceMap == nullptr));
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
void pedviewerWindow::setupConnections()
{
  connect(m_nextButton, SIGNAL(clicked()), 
          this, SLOT(nextPressed()));
  connect(m_prevButton, SIGNAL(clicked()), 
          this, SLOT(prevPressed()));
  connect(m_evtNumberEdit, SIGNAL(inputAccepted()), 
          this, SLOT(evtNumberEditChanged()));
  connect(m_menuBar, SIGNAL(dataFileLoaded(const QString&)),
          this, SLOT(openDataFile(const QString&)));
  connect(m_menuBar, SIGNAL(mapFileLoaded(const QString&)),
          this, SLOT(openMapFile(const QString&)));
  connect(m_menuBar, SIGNAL(referenceFileLoaded(const QString&)),
          this, SLOT(loadReferenceFile(const QString&)));
  connect (m_subtractRefCheckBox, SIGNAL(stateChanged(int)),
           this, SLOT(updatePlots()));
}
