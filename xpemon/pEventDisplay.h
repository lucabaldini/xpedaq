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

#ifndef PEVENTDISPLAY_H
#define PEVENTDISPLAY_H

#include <iostream>
#include <cmath>

#include <QMenu>

#include "qcustomplot.h"
#include "pHistogramOptions.h"
#include "pHexagonMatrix.h"
#include "pEvent.h"


class pEventDisplay : public QCustomPlot
{
  
  Q_OBJECT
  
  public:
  
    pEventDisplay(pColorMapOptions options = pColorMapOptions());
  
  public slots:
  
    void loadEvent(const pEvent& evt);
    void draw();
    // Restore the optimal visualization (change axis range so that
    // all data are visible and set the optimal color scale)
    virtual void resetView();
    void clearMap();
  
  signals:
  
    void dataRangeChanged(const QCPRange &dataRange);
  
  protected slots:
  
    void setDataRange (const QCPRange &dataRange);
    void updateAxesRange();
    void rowAxisUpdate(QCPRange range);
    void colAxisUpdate(QCPRange range);
    void updateDataRange();
    void updateColorScale();
    void updateMatrixColor();
    void drawMatrix();
    void mousePress();
    void mouseWheel();
    void selectionChanged();
    void contextMenuRequest(QPoint pos);
      
  protected:
    
    void setupInteractions();
    void pixelToCoord(int i, int j, double &x, double &y);
    void coordToPixel(double x, double y, int &i, int &j);
    
    pHexagonMatrix *m_hexMatrix;
    QCPRange m_dataRange;
    pEvent m_event;
    //event::Adc_vec_t m_AdcCounts;
    //int m_colMin;
    //int m_colMax;
    //int m_rowMin;
    //int m_rowMax;
    QCPColorScale *m_colorScale;
    QCPMarginGroup *m_marginGroup;
    pColorMapOptions m_options;
    bool m_isSyncronized;

};

#endif  //PEVENTDISPLAY_H
