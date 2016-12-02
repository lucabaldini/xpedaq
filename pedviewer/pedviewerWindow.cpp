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

pedviewerWindow::pedviewerWindow(QWidget *parent, int posx, int posy,
                                 int windowHeight, int windowWidth) :
  QMainWindow(parent),
  m_posx (posx), m_posy(posy),
  m_windowHeight (windowHeight),
  m_windowWidth (windowWidth)
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
}


/* Display the results (average and RMS) of a pedestal measurement
 */
void pedviewerWindow::showPedestals(const PedestalsMap& pedMap)
{  
  m_plotGrid -> fillPlots(pedMap);
  m_plotGrid -> replotAll();
}


/* Display the results (average and RMS) of a pedestal measurement
   subtracting a reference map
 */
void pedviewerWindow::showPedestals(const PedestalsMap& pedMap,
                                    const PedestalsMap& refMap)
{  
  m_plotGrid -> fillPlots(pedMap, refMap);
  m_plotGrid -> replotAll();
}


void pedviewerWindow::closeEvent(QCloseEvent *event)
{
  emit windowClosed();
  event->accept();
}
