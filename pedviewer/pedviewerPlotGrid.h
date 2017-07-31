#ifndef PEDSPLOTGRID_H
#define PEDSPLOTGRID_H

#include <iostream>

#include <QGridLayout>
#include <QSize>

#include "pHistogramOptions.h"
#include "pHistogramPlot.h"
#include "pHistogram.h"
#include "pMap.h"
#include "pMapPlot.h"
#include "pedestalsMap.h"


class PedviewerPlotGrid: public QWidget
{
  
  Q_OBJECT
  
  public:
  
    explicit PedviewerPlotGrid(QWidget *parent = 0);
    ~PedviewerPlotGrid();
    // Sets the preferred initial size
    QSize sizeHint() const {return QSize(730, 630);}
    
    enum displayMode {
      singleEvtType,
      mapType,
      unknownType
    };
  
  public slots:
    
    void fillPlots(const PedestalsMap& pedMap, displayMode plotType);
    void fillPlots(const PedestalsMap& pedMap, const PedestalsMap& refMap,
                   displayMode plotType);
    void replotAll();
    void clear();
     
  private:
    
    void findEdges(const PedestalsMap& pedMap,
                   double& aveMin, double& aveMax,
                   double& rmsMin, double& rmsMax) const;
    void findEdges(const PedestalsMap& pedMap, const PedestalsMap& refMap,
                   double& aveMin, double& aveMax,
                   double& rmsMin, double& rmsMax) const;
    void setupSupMap(const pColorMapOptions& option);
    void setupInfMap(const pColorMapOptions& option);
    void setupSupHist(const pBasicPlotOptions& options,
                      double xmin =0., double xmax =1800.);
    void setupInfHist(const pBasicPlotOptions& options,
                      double xmin =0., double xmax =150.);
    
    QGridLayout *m_PlotLayout;
    pMap *m_supMap;
    pMap *m_infMap;
    pHistogram *m_supHist;
    pHistogram *m_infHist;
    pMapPlot *m_supMapPlot;
    pMapPlot *m_infMapPlot;
    pHistogramPlot *m_supPlot;
    pHistogramPlot *m_infPlot;
};


#endif // PEDSPLOTGRID_H
