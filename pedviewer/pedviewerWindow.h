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

#ifndef PEDVIEWERWINDOW_H
#define PEDVIEWERWINDOW_H

#include <QtGui/QMainWindow>
#include <QtGui/QStatusBar>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

#include "pedestalsMap.h"
#include "pedviewerPlotGrid.h"


/* Class implementing a pop-up window to display a bunch of result plots
   at the end of an acquisition. */


class pedviewerWindow : public QMainWindow
{
  Q_OBJECT
  
  public:
  
    explicit pedviewerWindow(QWidget *parent = 0, int posx = 400,
                            int posy = 250, int windowWidth = 1200,
                            int windowWeight = 800);
  signals:
  
    void windowClosed();
  
  
  public slots:

    void showPedestals(const PedestalsMap& pedMap);
    void showPedestals(const PedestalsMap& pedMap,
                       const PedestalsMap& refMap);
  
  
  private slots:
 
   void closeEvent(QCloseEvent *event);
  
  
  private:
  
    int m_posx;
    int m_posy;
    int m_windowHeight;
    int m_windowWidth;
    QWidget *m_centralWidget;
    QVBoxLayout *m_verticalLayout;
    pedviewerPlotGrid *m_plotGrid;
};

#endif // PEDVIEWERWINDOW_H
