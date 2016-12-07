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

#include "pedviewerMenuBar.h"
#include "xpedaqos.h"

/*
*/
PedviewerMenuBar::PedviewerMenuBar(QWidget *parent) :
  QMenuBar(parent)
{
  createActions();
  createMenu();
  addMenu(m_fileMenu);
  std::string basePath = xpedaqos::environmentalVariable("XPEDAQ_ROOT");
  m_baseDir = QString::fromStdString(basePath);
}


/*
*/
void PedviewerMenuBar::openDataFilePressed()
{
  QString filePath = QFileDialog::getOpenFileName(this,
    tr("Open File"), m_baseDir, tr("Pedestal Files (*.mdat)"));
    //, 0, QFileDialog::DontUseNativeDialog);
  emit dataFileLoaded(filePath);
}


/*
*/
void PedviewerMenuBar::openMapFilePressed()
{
  QString filePath = QFileDialog::getOpenFileName(this,
    tr("Open File"), m_baseDir, tr("Pedestal Files (*.pmap)"));
    //, 0, QFileDialog::DontUseNativeDialog);
  emit mapFileLoaded(filePath);
}


/*
*/
void PedviewerMenuBar::loadReferenceMapPressed()
{
  QString filePath = QFileDialog::getOpenFileName(this,
    tr("Open Map"), m_baseDir, tr("Pedestal Maps (*.pmap)"));
    //, 0, QFileDialog::DontUseNativeDialog);
  emit referenceFileLoaded(filePath);
}


/*
*/
void PedviewerMenuBar::createActions()
{
    m_openDataFileAct = new QAction(tr("&Open data file"), this);
    m_openDataFileAct->setStatusTip(tr("Open a pedestals data file"));
    connect(m_openDataFileAct, SIGNAL(triggered()),
            this, SLOT(openDataFilePressed()));
    
    m_openMapFileAct = new QAction(tr("&Open map"), this);
    m_openMapFileAct->setStatusTip(tr("Open a pedestals map"));
    connect(m_openMapFileAct, SIGNAL(triggered()),
            this, SLOT(openMapFilePressed()));

    m_loadReferenceAct = new QAction(tr("&Load reference map"), this);
    m_loadReferenceAct->setStatusTip(tr("Load a reference pedestals map"));
    connect(m_loadReferenceAct, SIGNAL(triggered()),
            this, SLOT(loadReferenceMapPressed()));
}


/*
*/
void PedviewerMenuBar::createMenu()
{
    m_fileMenu = new QMenu(tr("&File"));
    addMenu(m_fileMenu);
    m_fileMenu->addAction(m_openDataFileAct);
    m_fileMenu->addAction(m_openMapFileAct);
    m_fileMenu->addAction(m_loadReferenceAct);
}
