/***********************************************************************
Copyright (C) 2007, 2008 by Luca Baldini (luca.baldini@pi.infn.it),
Johan Bregeon, Massimo Minuti and Gloria Spandre.

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

#ifndef PQTMAINWINDOWGUI_H
#define PQTMAINWINDOWGUI_H

#include <iostream>

#include <QMainWindow>
#include <QWidget>
#include <QGridLayout>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QStatusBar>


const int MAIN_GRID_LAYOUT_SPACING = 16;

class pQtMainWindowGui : public QMainWindow
{

 public:
  pQtMainWindowGui();
  ~pQtMainWindowGui() {;}
  void showMessage(QString message, int timeout = 0);

 protected:
  QGridLayout *m_mainGridLayout;
  QWidget *m_centralWidget;
  QMenu *m_fileMenu;
  QMenu *m_editMenu;
  QMenu *m_helpMenu;
  QAction *m_quitAction;

 private:
  void createActions();
  void createMenuBar();
};

#endif
