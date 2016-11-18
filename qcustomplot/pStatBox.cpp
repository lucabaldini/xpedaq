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

#include "pStatBox.h"


pStatBox::pStatBox(QCustomPlot *parent, double x0, double y0) :
  m_parent(parent),
  m_x0(x0),
  m_y0(y0)
{
  m_labelMap = new std::map<QString, QCPItemText *>();
  m_precisionMap = new std::map<QString, int>();
}


void pStatBox::addField(QString name, int precision)
{
  QCPItemText *label = new QCPItemText(m_parent);
  m_parent->addItem(label);
  m_labelMap->insert(std::pair<QString, QCPItemText*>(name, label));
  m_precisionMap->insert(std::pair<QString, int>(name, precision));
  label->setPositionAlignment(Qt::AlignTop | Qt::AlignLeft);
  label->position->setType(QCPItemPosition::ptAxisRectRatio);
  label->position->setCoords(m_x0, m_y0 + 0.075*m_labelMap->size());
}


void pStatBox::setField(QString name, double value)
{
  double precision = m_precisionMap->at(name);
  QString text = name + ": " + QString::number(value, 'f', precision);
  m_labelMap->at(name)->setText(text);
}


void pStatBox::setField(QString name, double value, double error)
{
  double precision = m_precisionMap->at(name);
  QString text = name + ": " + QString::number(value, 'f', precision) +
    " +/- " + QString::number(error, 'f', precision);
  m_labelMap->at(name)->setText(text);
}


void pStatBox::reset()
{
  std::map<QString, QCPItemText*>::iterator it;
  for (it = m_labelMap->begin(); it != m_labelMap->end(); ++it) {
    QString text = it->first + ": -";
    it->second->setText(text);
  }
}
