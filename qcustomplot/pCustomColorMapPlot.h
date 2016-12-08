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

#ifndef PCUSTOMCOLORMAPPLOT_H
#define PCUSTOMCOLORMAPPLOT_H

#include <QMenu>
#include <QFileDialog>
#include <QString>

#include "qcustomplot.h"
#include "pHistogramOptions.h"

class pCustomColorMapPlot : public QCustomPlot
{
  
  Q_OBJECT
  
  public:
  
    pCustomColorMapPlot(pColorMapOptions options = pColorMapOptions());    
    void setCellContent(unsigned int xCell, unsigned int yCell, double value);
    void setDataContent(double x, double y, double value);    
    void setRange (double xmin, double xmax, double ymin, double ymax);
    void rescaleDataRange() {m_colorMap -> rescaleDataRange();}
    void setSize  (unsigned int nXbins, unsigned int nYbins)
      {m_data->setSize(nXbins, nYbins);}
    void clearMap();
    void setInterpolate(bool interpolate);
  
  public slots:
  
    // Restore the optimal visualization (change axis range so that
    // all data are visible and set the optimal color scale)
    virtual void resetView();
  
  protected slots:
  
    void mousePress();
    void mouseWheel();
    void selectionChanged();
    void mouseMoveEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);
    virtual void paintCoordinate();
    void contextMenuRequest(QPoint pos);
    void setLogScaleZ();
    void setLinScaleZ();
    void savePlot();
     
  protected:
    
    void setupInteractions();
   
    QCPColorMap *m_colorMap;
    QCPColorScale *m_colorScale;
    QCPMarginGroup *m_marginGroup;
    QCPColorMapData *m_data;  
    pColorMapOptions m_options;
    bool m_isLogScaleZ;
    QPoint m_cursorPos;
};

#endif  //PCUSTOMCOLORMAPPLOT_H
