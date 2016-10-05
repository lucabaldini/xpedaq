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
