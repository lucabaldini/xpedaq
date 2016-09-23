#ifndef PIXELMAP_H
#define PIXELMAP_H

#include <QMenu>

#include "qcustomplot.h"
#include "pHistogramOptions.h"
#include "pHexagonMatrix.h"

#define P_C 0.05 // column pitch [mm] of the ASIC

class pEventDisplay : public QCustomPlot
{
  
  Q_OBJECT
  
  public:
  
    pEventDisplay(pColorMapOptions options = pColorMapOptions());
  
  public slots:
  
    void setAdcData(const std::vector<double> &values);
    void setWindowRange (unsigned int firstCol, unsigned int lastCol,
                         unsigned int firstRow, unsigned int lastRow);
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
    void pixelToCoord(unsigned int i, unsigned int j, double &x, double &y);
    
    pHexagonMatrix *m_hexMatrix;
    std::vector<double> m_AdcCounts;
    QCPRange m_dataRange;
    unsigned int m_colMin;
    unsigned int m_colMax;
    unsigned int m_rowMin;
    unsigned int m_rowMax;
    QCPColorScale *m_colorScale;
    QCPMarginGroup *m_marginGroup;
    pColorMapOptions m_options;
    bool m_isSyncronized;

};

#endif  //CUSTOMCOLORMAPPLOT_H
