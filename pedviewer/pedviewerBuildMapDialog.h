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

#ifndef PEDVIEWERBUILDMAPDIALOG
#define PEDVIEWERBUILDMAPDIALOG

#include <vector>

#include <QWidget>
#include <QDialog>
#include <QPushButton>
#include <QGridLayout>

#include "pQtCustomLineEdit.h"
#include "pQtCustomTextLabel.h"
#include "pMinMaxOptionPair.h"

/* \brief Dialog window for querying the user a list of events to be used
          to build a new PedestalsMap
*/

class PedviewerBuildMapDialog : public QDialog
{
  Q_OBJECT
  
  public:
    PedviewerBuildMapDialog(QWidget* parent, int evtMax);
  
  signals:
    void inputAccepted(const std::vector<int>& events);
  
  private slots:
    void readInput();
    void condAllowOk();
  
  private:
    std::vector<int> m_events;
    pMinMaxOptionPair<int>* m_pageRange;
    QPushButton* m_okButton;
    QPushButton* m_cancelButton;
};

#endif // PEDVIEWERBUILDMAPDIALOG
