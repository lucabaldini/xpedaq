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

#include "pTriggerSettingTab.h"

/*!
 */
pTriggerSettingTab::pTriggerSettingTab()
  : pQtCustomTab("Trigger mask")
{
  int row = 0;
  m_tableWidget = new QTableWidget(0, 2);
  m_tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
  m_tableWidget->setFixedHeight(300);
  QStringList tableHeader;
  tableHeader << "x" << "y";
  m_tableWidget->setHorizontalHeaderLabels(tableHeader);
  //m_tableWidget->horizontalHeader()->setResizeMode(0, QHeaderView::Stretch);
  //m_tableWidget->horizontalHeader()->setResizeMode(1, QHeaderView::Stretch);
  m_tableWidget->horizontalHeader()->setStretchLastSection(true);
  m_groupBoxGridLayout->addWidget(m_tableWidget, row, 0);
  freezeSize(xpolgui::kTabGroupBoxWidth);
  //setContextMenuPolicy(Qt::CustomContextMenu);
  //connect(this, SIGNAL(customContextMenuRequested(const QPoint&)),
  //	  this, SLOT(showContextMenu(const QPoint&)));
}


/*!
 */
void pTriggerSettingTab::showContextMenu(const QPoint& pos)
{
  QPoint globalPos = mapToGlobal(pos);
  QMenu menu;
  menu.addAction("Add channel");
  menu.addAction("Delete channel");
  QAction* selectedItem = menu.exec(globalPos);
  if (selectedItem) {
    // something was chosen, do stuff
  } else {
    // nothing was chosen
  }
}


/*!
 */
void pTriggerSettingTab::displayTriggerMask(pTriggerMask *triggerMask)
{
  TriggerMask_t *mask = triggerMask->mask();
  m_tableWidget->setRowCount(triggerMask->size());
  int row = 0;
  for (const auto &chan : *mask) {
    unsigned short x = chan.first;
    unsigned short y = chan.second;
    m_tableWidget->setItem(row, 0, new QTableWidgetItem(QString::number(x)));
    m_tableWidget->setItem(row, 1, new QTableWidgetItem(QString::number(y)));
    m_tableWidget->item(row, 0)->setTextAlignment(Qt::AlignRight);
    m_tableWidget->item(row, 1)->setTextAlignment(Qt::AlignRight);
    row ++;
  }
}


/*!
 */
pTriggerMask *pTriggerSettingTab::triggerMask() const
{
  pTriggerMask *triggerMask = new pTriggerMask();
  for (int row = 0; row < m_tableWidget->rowCount(); row++) {
    unsigned short x = m_tableWidget->item(row, 0)->text().toInt();
    unsigned short y = m_tableWidget->item(row, 1)->text().toInt();
    if ((x <= xpoldetector::kNumPixelsX) & (y <= xpoldetector::kNumPixelsY)) {
      triggerMask->add(x, y);
    } else {
      *xpollog::kError << "Invalid trigger mask entry (" << x << ", " << y
		       << "), skipping..." << endline;
    }
  }
  return triggerMask;
}
