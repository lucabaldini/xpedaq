#ifndef DISPLAYWINDOW_H
#define DISPLAYWINDOW_H

#include <QtGui/QMainWindow>
#include <QtGui/QStatusBar>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

#include "pMapPlot.h"
#include "pedestalsMap.h"
#include "pDataBlock.h"


/* Class implementing a pop-up window to display a bunch of result plots
   at the end of an acquisition.
   
   TODO: Display these plots directly into the gui and get rid of this
   additional window. */


class pDisplayWindow : public QMainWindow
{
  Q_OBJECT
  
  public:
  
    explicit pDisplayWindow(QWidget *parent = 0, int posx = 400,
                            int posy = 250, int windowWidth = 800,
                            int windowWeight = 600);
    
  public slots:

    void showPedestals(const PedestalsMap& pedMap);  
  
  private slots:
 
  
  private:
  
    int m_posx;
    int m_posy;
    int m_windowHeight;
    int m_windowWidth;
    QWidget *m_centralWidget;
    QVBoxLayout *m_verticalLayout;
    QStatusBar *m_statusBar; 
    pMapPlot *m_meanPlot;
    pMapPlot *m_rmsPlot;
};

#endif // DISPLAYWINDOW_H
