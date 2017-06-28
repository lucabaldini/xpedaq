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

#include <QApplication>
#include <QtCore>

#include "xpedaqutils.h"
#include "pedviewerWindow.h"


/* Application for viewing a file produced by the pedestals application
   (aka "xpepeds").
   Can be used to open:
   1) a ".pmap" file 
   2) a ".mdat" file
   A reference map (".pmap" file) can be loaded and used for comparison.  
*/
int main(int argn, char *argv[])
{
  xpedaqutils::startmsg();
  
  // Start the application.
  QApplication app(argn, argv);
  
  // Create the window.
  pedviewerWindow* window = new pedviewerWindow();
  QObject::connect(window, SIGNAL(windowClosed()),
	                 &app, SLOT(quit()));
  // Show the window
  window -> show();
  return app.exec();
}