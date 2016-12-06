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
void PedviewerMenuBar::openFilePressed()
{
  QString filePath = QFileDialog::getOpenFileName(this,
    tr("Open File"), m_baseDir, tr("Pedestal Files (*.pmap *.mdat)"));
    //, 0, QFileDialog::DontUseNativeDialog);
  emit fileLoaded(filePath);
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
    m_openAct = new QAction(tr("&Open File"), this);
    m_openAct->setStatusTip(tr("Open a pedestal data file or map"));
    connect(m_openAct, SIGNAL(triggered()), this, SLOT(openFilePressed()));
    m_loadReferenceAct = new QAction(tr("&Load reference map"), this);
    m_loadReferenceAct->setStatusTip(
                               tr("Open a pedestal map to use as reference"));
    connect(m_loadReferenceAct, SIGNAL(triggered()),
            this, SLOT(loadReferenceMapPressed()));
}


/*
*/
void PedviewerMenuBar::createMenu()
{
    m_fileMenu = new QMenu(tr("&File"));
    addMenu(m_fileMenu);
    m_fileMenu->addAction(m_openAct);
    m_fileMenu->addAction(m_loadReferenceAct);
}
