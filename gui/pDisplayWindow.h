#ifndef DISPLAYWINDOW_H
#define DISPLAYWINDOW_H

#include <QtGui/QMainWindow>
#include <QtGui/QStatusBar>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include "qcustomplot.h"
#include "pedestalsMap.h"


/* Class implementing a pop-up window to display a bunch of result plots
   at the end of an acquisition.
   
   TO DO: Display these plots directly into the gui and get rid of this
   additional window. */


class pDisplayWindow : public QMainWindow
{
  Q_OBJECT
  
  public:
    explicit pDisplayWindow(QWidget *parent = 0, int posx = 400,
                            int posy = 250, int windowWidth = 800,
                            int windowWeight = 600);
    
  private slots:
    void showPedestals();
  
  private:
    int m_posx;
    int m_posy;
    int m_windowHeight;
    int m_windowWidth;
    QWidget *m_centralWidget;
    QVBoxLayout *m_verticalLayout;
    QStatusBar *m_statusBar; 
    QCustomPlot *m_pedestalPlot;
    QCustomPlot *m_pedestalRmsPlot;
    PedestalsMap *m_pedestalMap;
    PedestalsMap *m_pedestalRmsMap;
};

#endif // DISPLAYWINDOW_H
