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

#include "pQtGridLayoutWidget.h"


pQtGridLayoutWidget::pQtGridLayoutWidget(QWidget *parent)
  : QWidget(parent)
{
  m_mainGridLayout = new QGridLayout(this);
  m_mainGridLayout->setMargin(0);
}

void pQtGridLayoutWidget::addWidget(QWidget *widget, int row, int column)
{
  m_mainGridLayout->addWidget(widget, row, column);
}

void pQtGridLayoutWidget::addItem(QLayoutItem *item, int row, int column)
{
  m_mainGridLayout->addItem(item, row, column);
}

void pQtGridLayoutWidget::freezeWidth(int width)
{
  adjustSize();
  int height = size().height();
  if (width == -1){
    width = size().width();
  }
  setFixedSize(width, height);
}

void pQtGridLayoutWidget::freezeHeight(int height)
{
  adjustSize();
  int width = size().width();
  if (height == -1){
    height = size().height();
  }
  setFixedSize(width, height);
}

void pQtGridLayoutWidget::freezeSize(int width, int height)
{
  adjustSize();
  if (width == -1){
    width = size().width();
  }
  if (height == -1){
    height = size().height();
  }
  setFixedSize(width, height);
}
