#ifndef DISPLAYWINDOW_H
#define DISPLAYWINDOW_H

#include <QtGui/QMainWindow>
#include <QtGui/QStatusBar>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

#include "pQtCustomPushButton.h"
#include "pedestalsMap.h"
#include "xpepedsPlotGrid.h"


/* Class implementing a pop-up window to display a bunch of result plots
   at the end of an acquisition. */


class pDisplayWindow : public QMainWindow
{
  Q_OBJECT
  
  public:
  
    explicit pDisplayWindow(QWidget *parent = 0, int posx = 400,
                            int posy = 250, int windowWidth = 1200,
                            int windowWeight = 800);
    
  
  signals:
  
    void windowClosed();
  
  
  public slots:

    void showPedestals(const PedestalsMap& pedMap);
  
  
  private slots:
 
   void closeEvent(QCloseEvent *event);
  
  
  private:
  
    int m_posx;
    int m_posy;
    int m_windowHeight;
    int m_windowWidth;
    QWidget *m_centralWidget;
    QVBoxLayout *m_verticalLayout;
    xpepedsPlotGrid *m_plotGrid;
    QStatusBar *m_statusBar;
};

#endif // DISPLAYWINDOW_H
