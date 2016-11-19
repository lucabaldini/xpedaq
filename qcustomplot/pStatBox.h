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

#ifndef PSTATBOX_H
#define PSTATBOX_H

#include <string>
#include <map>
#include "qcustomplot.h"


class pStatBox
{
  
  public:
  
    pStatBox(QCustomPlot *parent, double x0, double y0);
    void addField(QString name, int precision);
    void setField(QString name, double value, QString units = "");
    void setField(const char *name, double value, QString units = "");
    void setField(QString name, double value, double error, QString units = "");
    void setField(const char *name, double value, double error,
		  QString units = "");
    void reset();
    
  private:

    QCustomPlot *m_parent;
    double m_x0;
    double m_y0;
    std::map<QString, QCPItemText *> *m_labelMap;
    std::map<QString, int> *m_precisionMap;
};


#endif // PSTATBOX_H
