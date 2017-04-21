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

#include "pDataDisplayWidget.h"


/*! \brief Basic constructor.
 */
pDataDisplayWidget::pDataDisplayWidget(const QString &title,
				       QWidget *parent = 0) :
  QGroupBox(title, parent)
{
  m_widgetMap = new std::map<const QString, QLabel*>();
  m_gridLayout = new QGridLayout(this);
  // We want a tight vertical layout for the data display.
  m_gridLayout->setSpacing(1);
}


/*! \brief Constructor with default title.
 */
pDataDisplayWidget::pDataDisplayWidget(QWidget *parent = 0) :
  pDataDisplayWidget("Data acquisition information", parent) {}


/*! \brief Add a field to the display widget.
 */
void pDataDisplayWidget::addField(const QString &name, const QString &label)
{
  // Create the two QLabel objects (for the text label and the actual data).
  QLabel *labelWidget = new QLabel(label, this);
  QLabel *dataWidget = new QLabel(xpolgui::kNotAvailable, this);
  // Format the data widget properly.
  dataWidget->setAlignment(Qt::AlignRight);
  dataWidget->setFrameShape(QFrame::StyledPanel);
  dataWidget->setAutoFillBackground(1);
  setBackgroundRole(QPalette::BrightText);
  // Fill the underlying widget map, so that we can retrieve the objects later.
  m_widgetMap->insert(std::make_pair(name, dataWidget));
  // Finally, insert the two widgets in the layout at the current row.
  int row = m_gridLayout->rowCount();
  m_gridLayout->addWidget(labelWidget, row, 0);
  m_gridLayout->addWidget(dataWidget, row, 1);
}


/*! \brief Add the standard data acquisition fields to the display, namely the
  station and run identifiers and the data acquisition status.
 */
void pDataDisplayWidget::addDataAcquisitionFields()
{
  addStationId();
  addRunId();
  addStatus();
  addElapsedSeconds();
}


/*! \brief Return a pointer to the data widget corresponding to a given field
  (identified by name).
 */
QLabel* pDataDisplayWidget::dataWidget(const QString &name)
{
  try {
    return m_widgetMap->at(name);
  } catch (const std::out_of_range& e) {
    *xpollog::kError << "Invalid name (" << name <<
      ") passed to pDataDisplayWidget::dataWidget()." << endline;
    exit(1);
  }
}


/*! \brief Set the value for a given data field.
 */
void pDataDisplayWidget::set(const QString &name, QString value)
{
  dataWidget(name)->setText(value);
}


/*! \brief Set the value for a given data field.
 */
void pDataDisplayWidget::set(const QString &name, int value)
{
  dataWidget(name)->setText(QString::number(value));
}


/*! \brief Set the value for a given data field.
 */
void pDataDisplayWidget::set(const QString &name, double value,
			     int precision = 2)
{
  dataWidget(name)->setText(QString::number(value, 'f', precision));
}
