#ifndef PEDSPLOTGRID_H
#define PEDSPLOTGRID_H

#include <iostream>

#include <QGridLayout>
#include <QSize>
#include <QPushButton>
#include <QHBoxLayout>

#include "pHistogramOptions.h"
#include "pHistogramPlot.h"
#include "pHistogram.h"
#include "pMap.h"
#include "pMapPlot.h"
#include "pedestalsMap.h"
#include "pQtCustomLineEdit.h"
#include "pQtCustomTextLabel.h"


class pedviewerPlotGrid: public QWidget
{
  
  Q_OBJECT
  
  public:
  
    explicit pedviewerPlotGrid(QWidget *parent = 0);
    // Sets the preferred initial size
    QSize sizeHint() const {return QSize(800, 800);}
    
    // Public members, for easer connection in the GUI
    QPushButton *m_prevButton;
    QPushButton *m_nextButton;
    pQtCustomLineEdit<int> *m_evtNumberEdit; 
  
  public slots:
    
    void fillPlots(const PedestalsMap& pedMap);
    void fillPlots(const PedestalsMap& pedMap, const PedestalsMap& refMap);
    void replotAll();
    void setButtonsEnabled(bool enabled);
    void setNextButtonEnabled(bool enabled);
    void setPrevButtonEnabled(bool enabled);
    void setTotEvtLabel(int numEvents);
    void updateEvtNumberEdit(int curEvent);
    void clear();
     
  private:
    
    void setupSupMap(const pColorMapOptions& option);
    void setupInfMap(const pColorMapOptions& option);
    void setupSupHist(const pBasicPlotOptions& options,
                      double xmin =0., double xmax =1800.);
    void setupInfHist(const pBasicPlotOptions& options,
                      double xmin =0., double xmax =150.);
    
    pMap *m_supMap;
    pMap *m_infMap;
    pHistogram *m_supHist;
    pHistogram *m_infHist;
    QGridLayout *m_PlotLayout;
    pMapPlot *m_supMapPlot;
    pMapPlot *m_infMapPlot;
    pHistogramPlot *m_supPlot;
    pHistogramPlot *m_infPlot;
    QHBoxLayout *m_navLayout;
    pQtCustomTextLabel *m_totEvtLabel;
};


#endif // PEDSPLOTGRID_H
