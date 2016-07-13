#ifndef CUSTOMHISTOGRAMPLOT_H
#define CUSTOMHISTOGRAMPLOT_H

#include <vector>

#include <QMenu>

#include "qcustomplot.h"
#include "pHistogramOptions.h"

class pCustomHistogramPlot : public QCustomPlot
{
  
  Q_OBJECT
  
  public:
  
    pCustomHistogramPlot(pBasicPlotOptions options = pBasicPlotOptions(),
                         bool logScaleY = false);      
    
    void setKeyContent(double key, double value);
    void setTolerance (double tolerance);
    void clearBars();
    
  public slots:
  
    // Restore the optimal visualization (change axis range so that
    // all data are visible)
    virtual void resetView();


  protected slots:
  
    void mousePress();
    void mouseWheel();
    void selectionChanged();
    void mouseMoveEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);
    void paintCoordinate();
    void contextMenuRequest(QPoint pos);
    void setLogScaleY();
    void setLinScaleY();
      
  protected:
    
    void setupInteractions();
   
    QCPBars *m_bars;
    pBasicPlotOptions m_options;    
    // Needed to properly remove key in QCPBars data
    double m_centerPosTolerance;
    bool m_isLogScaleY;
    QPoint m_cursorPos;

};

#endif  //CUSTOMHISTOGRAMPLOT_H
