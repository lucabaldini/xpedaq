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
  m_centralWidget(nullptr),
  m_referencePlotWidget(nullptr),
  m_tabWidget(nullptr),
  m_verticalLayout(nullptr),
  m_plotGrid(nullptr),
  m_refPlotGrid(nullptr),
  m_menuBar(nullptr),
  m_inputFile(nullptr),
  m_pedMap(nullptr),
  m_referenceMap(nullptr),
  m_curEvent(-1),
  m_plotType(PedviewerPlotGrid::unknownType)
{
  if (objectName().isEmpty())
    setObjectName(QString::fromUtf8("DisplayWindow"));
  resize(m_windowHeight, m_windowWidth);
  m_centralWidget = new QWidget(this);
  m_centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
  setCentralWidget(m_centralWidget);
  
  //Apparently we need this totally useless intermediate layout, or the
  //tab widget won't resize automatically with the main window...
  QVBoxLayout* dummyLayout = new QVBoxLayout(m_centralWidget);
  
  m_tabWidget = new QTabWidget(m_centralWidget);
  dummyLayout->addWidget(m_tabWidget);
  
  QWidget* mainPlotWidget = new QWidget();
  m_tabWidget->addTab(mainPlotWidget, "Plots");
  
  m_verticalLayout = new QVBoxLayout(mainPlotWidget);
  m_verticalLayout->setContentsMargins(10, 10, 10, 10);
  m_verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));

  m_menuBar = new PedviewerMenuBar();
  m_verticalLayout->setMenuBar(m_menuBar);
  setupNavBar();
  setNavBarEnabled(false);

  m_plotGrid = new PedviewerPlotGrid(mainPlotWidget);
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
  m_numEvents = m_inputFile->nEvents();
  // Setup navigation bar
  setTotEvtLabel(m_numEvents);
  setEvtNumberEditRange(1, m_numEvents);
  // Plot the first event
  m_plotType = PedviewerPlotGrid::singleEvtType;
  showEvent(1);
  m_menuBar->setBuildMapActionEnabed(false);
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
  m_numEvents = m_inputFile->nEvents();
  // Setup navigation bar
  setNavBarEnabled(false);
  setEvtNumberEditRange(1, m_numEvents);
  setTotEvtLabel(m_numEvents);
  updateNavBarStatus(m_numEvents);
  // Plot the map
  m_plotType = PedviewerPlotGrid::mapType;
  showMap();
  m_menuBar->setBuildMapActionEnabed(false);
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
  // If there is a main file opened enable the check box
  if (!(m_inputFile==nullptr)){
    m_subtractRefCheckBox->setEnabled(true);
    // If the check box was already checked apply the subtraction immediatly
    updatePlots();   
  }
  // Show the reference map in a separate tab
  if (m_referencePlotWidget == nullptr){
    // If the tab hasn't already been created do it now
    m_referencePlotWidget = new QWidget();
    m_tabWidget->addTab(m_referencePlotWidget, "Reference");
    QVBoxLayout *refVerticalLayout = new QVBoxLayout(m_referencePlotWidget);
    refVerticalLayout->setContentsMargins(10, 10, 10, 10);
    refVerticalLayout->setObjectName(QString::fromUtf8("verticalRefLayout"));
    m_refPlotGrid = new PedviewerPlotGrid(m_referencePlotWidget);
    refVerticalLayout->addWidget(m_refPlotGrid, 0);
  } else {
    m_refPlotGrid->clear();
  }
  m_refPlotGrid->fillPlots((*m_referenceMap), PedviewerPlotGrid::mapType);
  // Close te file and release the memory
  delete referenceFile;
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
  updateNavBarStatus(m_numEvents);
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
  m_plotGrid->clear();
  if (m_subtractRefCheckBox->isChecked()){
    m_plotGrid->fillPlots(*m_pedMap, *m_referenceMap, m_plotType);
  } else {
    m_plotGrid->fillPlots(*m_pedMap, m_plotType);
  }
  m_plotGrid->replotAll();
}


/*
*/
void pedviewerWindow::updateNavBarStatus(int curEvent)
{
  if (curEvent == 1) m_prevButton->setEnabled(false);
  if (curEvent == m_numEvents) m_nextButton->setEnabled(false);
  updateEvtNumberEdit(curEvent);
  // Activate the checkbox only if there is a reference map loaded
  m_subtractRefCheckBox->setEnabled(!(m_referenceMap == nullptr));
}


/*
*/
void pedviewerWindow::showBuildMapDialog()
{
  PedviewerBuildMapDialog* buildMapDialog = new PedviewerBuildMapDialog(this,
    m_numEvents);
  connect (buildMapDialog, SIGNAL(inputAccepted(const std::vector<int>&)),
           this, SLOT(buildAndShowMap(const std::vector<int>&)));           
  // Show the dialog (modal)
  buildMapDialog->exec();
}


/*
*/
void pedviewerWindow::buildAndShowMap(const std::vector<int>& events)
{
  *xpollog::kInfo << events.at(0) << endline;
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
  /* Menu bar connections */
  connect(m_evtNumberEdit, SIGNAL(inputAccepted()), 
          this, SLOT(evtNumberEditChanged()));
  connect(m_menuBar, SIGNAL(dataFileLoaded(const QString&)),
          this, SLOT(openDataFile(const QString&)));
  connect(m_menuBar, SIGNAL(mapFileLoaded(const QString&)),
          this, SLOT(openMapFile(const QString&)));
  connect(m_menuBar, SIGNAL(referenceFileLoaded(const QString&)),
          this, SLOT(loadReferenceFile(const QString&)));
  connect (m_menuBar, SIGNAL(buildMapPressed()),
           this, SLOT(showBuildMapDialog()));
  
  /* Navigation bar connections */
  connect(m_nextButton, SIGNAL(clicked()), 
          this, SLOT(nextPressed()));
  connect(m_prevButton, SIGNAL(clicked()), 
          this, SLOT(prevPressed()));
  connect (m_subtractRefCheckBox, SIGNAL(stateChanged(int)),
           this, SLOT(updatePlots()));
}
