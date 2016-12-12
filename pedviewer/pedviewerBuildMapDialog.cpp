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

#include "pedviewerBuildMapDialog.h"

/*
*/
PedviewerBuildMapDialog::PedviewerBuildMapDialog(QWidget* parent, int evtMax): 
  QDialog (parent)
{ 
  // Create the graphical objects inside the dialog window
  QGridLayout* dialogLayout = new QGridLayout(this);
  this->setLayout(dialogLayout);
  m_okButton = new QPushButton("Ok");
  m_cancelButton = new QPushButton("Cancel");
  m_pageRange = new pMinMaxOptionPair<int>(this, 1, evtMax);
  m_pageRange->setBottom(1);
  m_pageRange->setTop(evtMax);
  dialogLayout->addWidget(m_pageRange->minEdit(), 0, 0);
  dialogLayout->addWidget(m_pageRange->maxEdit(), 0, 1);
  dialogLayout->addWidget(m_okButton, 1, 0);
  dialogLayout->addWidget(m_cancelButton, 1, 1);
  // Setup connections
  connect(m_okButton, SIGNAL(clicked()), this, SLOT(readInput()));
  connect(m_cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
  connect(m_pageRange, SIGNAL(textEdited()), this, SLOT(condAllowOk()));
}


/*
*/
void PedviewerBuildMapDialog::readInput()
{
  int min, max;
  m_pageRange->readOptions(min, max);
  for (int i = min; i<=max; ++i){
    m_events.push_back(i);
  }
  emit inputAccepted(m_events);
  this->accept();
}


/*
*/
void PedviewerBuildMapDialog::condAllowOk()
{
  //m_pageRange->minEdit()->validate();
  //m_pageRange->maxEdit()->validate();
  int min, max;
  m_pageRange->readOptions(min, max);
  m_okButton->setEnabled(min < max);  
}
