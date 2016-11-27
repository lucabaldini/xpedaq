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

#ifndef PDATADISPLAYWIDGET_H
#define PDATADISPLAYWIDGET_H

#include <map>
#include <iostream>

#include <QWidget>
#include <QLabel>
#include <QGroupBox>
#include <QGridLayout>

#include "xpolgui.h"
#include "xpollog.h"


class pDataDisplayWidget : public QGroupBox
{

  Q_OBJECT
  
  public:

    pDataDisplayWidget(const QString &title, QWidget *parent);
    pDataDisplayWidget(QWidget *parent);

    void addStationId() {addField("station_id", "Station ID");}
    void addRunId() {addField("run_id", "Run Id");}
    void addStatus() {addField("status", "Status");}
    void addElapsedSeconds() {addField("elapsed_seconds", "Elapsed time [s]");}
    void addAverageRate() {addField("average_rate", "Average rate [Hz]");}
    void addNumEvents() {addField("num_events", "Number of events acquired");}
    void addNumDataBlocks() {addField("num_data_blocks",
				      "Number of data blocks acquired");}
    void addNumErrors() {addField("num_errors", "Number of readout errors");}
    void addNumPokes() {addField("num_pokes", "Number of register pokes");}
    void addNumReadouts() {addField("num_readouts", "Number of readouts");}
    void addDataAcquisitionFields();

  public slots:
   
    void set(const QString &name, QString value);
    void set(const QString &name, int value);
    void set(const QString &name, double value, int precision);

    void setStationId(int value) {set("station_id", value);}
    void setRunId(int value) {set("run_id", value);}
    void setStatus(QString value) {set("status", value);}
    void setElapsedSeconds(int value) {set("elapsed_seconds", value);}
    void setAverageRate(double value) {set("average_rate", value);}
    void setNumEvents(int value) {set("num_events", value);}
    void setNumDataBlocks(int value) {set("num_data_blocks", value);}
    void setNumErrors(int value) {set("num_errors", value);}
    void setNumPokes(int value) {set("num_pokes", value);}
    void setNumReadouts(int value) {set("num_readouts", value);}
      
      
  private:
  
    QGridLayout *m_gridLayout;
    std::map<const QString, QLabel*> *m_widgetMap;
    void addField(const QString &name, const QString &label);
    QLabel *dataWidget(const QString &name);
};

#endif //PDATADISPLAYWIDGET_H
