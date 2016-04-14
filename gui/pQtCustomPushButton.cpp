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

# include "pQtCustomPushButton.h"

pQtCustomPushButton::pQtCustomPushButton(QWidget *parent,
					 std::string functionName)
  : QPushButton(parent), m_functionName(functionName)
{
  std::string objectName = m_functionName + "_button";
  setObjectName(QString::fromStdString(objectName));
  setupIcon();
  setFixedSize(xpolgui::kButtonsWidth, xpolgui::kButtonsHeight);
}

void pQtCustomPushButton::setupIcon()
{
  std::string basePixmapPath = xpolenv::join(xpolenv::kDaqPixmapsDirPath,
					     m_functionName);
  std::string enabledPixmapPath = basePixmapPath + "_enabled.png";
  std::string disabledPixmapPath = basePixmapPath + "_disabled.png";
  QIcon icon = QIcon(QString::fromStdString(enabledPixmapPath));
  icon.addPixmap(QPixmap(QString::fromStdString(disabledPixmapPath)),
		 QIcon::Disabled);
  setIcon(icon);
  setIconSize(QSize(xpolgui::kButtonsIconsSize, xpolgui::kButtonsIconsSize));
}
