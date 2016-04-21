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

#include "pQtDisplayGui.h"

pQtDisplayGui::pQtDisplayGui(QWidget *parent)
  : pQtGroupBoxWidget(parent)
{
  m_groupBoxGridLayout->setSpacing(1);
}

pQtCustomTextLabel *pQtDisplayGui::addField(QString fieldLabel)
{
  int row = m_groupBoxGridLayout->rowCount();
  pQtCustomTextLabel *dataWidget =
    new pQtCustomTextLabel(this, xpolgui::kNotAvailable);
  pQtCustomTextLabel *labelWidget = new pQtCustomTextLabel(this, fieldLabel);
  dataWidget->setDataStyle();
  labelWidget->setLabelStyle(xpolgui::kDaqDisplayLabelsWidth);
  m_groupBoxGridLayout->addWidget(labelWidget, row ,0);
  m_groupBoxGridLayout->addWidget(dataWidget, row ,1);
  return dataWidget;
}

