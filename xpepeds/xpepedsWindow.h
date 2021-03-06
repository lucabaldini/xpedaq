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

#ifndef XPEPEDSWINDOW_H
#define XPEPEDSWINDOW_H

#include "pAcquisitionWindow.h"
#include "pDisplayWindow.h"
#include "pedRunController.h"

class xpepedsWindow : public pAcquisitionWindow
{

  Q_OBJECT

 public:
  
  xpepedsWindow(pedRunController &runController);
  ~xpepedsWindow() {;}
  
  virtual pDetectorConfiguration *detectorConfiguration(int mode = -1);
 
 
 public slots:  
 
 
 private slots:

   void disableUnusedWidgets();
   void showDisplayWindow();
   void closeDisplayWindow(); 
   void displayClosed();
 
 
 private:
  
  pDisplayWindow *m_displayWindow;
  bool m_isWindowOpen;
  pedRunController *m_pedRunController;
  
  void setupConnections();

};

#endif //XPEPEDSWINDOW_H
