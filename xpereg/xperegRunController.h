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


#ifndef XPEREGRUNCONTROLLER_H
#define XPEREGRUNCONTROLLER_H

#include <iostream>

#include <QThread>

#include "pRunController.h"
#include "pXpolRegisterPoker.h"
#include "xperegUserPreferences.h"

   
class xperegRunController: public pRunController
{

  Q_OBJECT 
   
  public:
    
    xperegRunController(std::string preferencesFilePath);
    void setupRun(xperegUserPreferences *preferences);
    void setupRun(std::string preferencesFilePath);
    void setupRun();
    xperegUserPreferences *userPreferences() const {return m_userPreferences;}
    
  public slots:

  protected:
  
    virtual void fsmSetup();
    virtual void fsmTeardown();
    virtual void fsmStartRun();
    virtual void fsmStopRun();
    virtual void fsmPause();
    virtual void fsmResume();
    virtual void fsmStop();

  private:

    // Why the hell we're not picking up the one from the base class?
    std::string m_preferencesFilePath;
    xperegUserPreferences *m_userPreferences;
    pXpolRegisterPoker *m_registerPoker;
    QThread m_thread;
};

#endif //XPEREGRUNCONTROLLER_H
