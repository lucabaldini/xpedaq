#ifndef CUSTOMCOLORMAPPLOT_H
#define CUSTOMCOLORMAPPLOT_H

#include <QMenu>

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
    void paintCoordinate();
    void contextMenuRequest(QPoint pos);
    void setLogScaleZ();
    void setLinScaleZ();
      
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

#endif  //CUSTOMCOLORMAPPLOT_H
