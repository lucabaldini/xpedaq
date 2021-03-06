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

#include <QWidget>
#include <QMenuBar>
#include <QMenu>
#include <QString>

#ifndef PEDVIEWERMENUBAR_H
#define PEDVIEWERMENUBAR_H

class PedviewerMenuBar : public QMenuBar
{
  
  Q_OBJECT
  
  public:
    explicit PedviewerMenuBar(QWidget* parent =0);   
    void setBuildMapActionEnabed(bool enabled);
    
  signals:
    void dataFileLoaded(const QString& filePath) const;
    void mapFileLoaded(const QString& filePath) const;
    void referenceFileLoaded(const QString& filePath) const;
    void buildMapPressed();
    
  public slots:
    void openDataFilePressed();
    void openMapFilePressed();
    void loadReferenceMapPressed();
  
  private:
    void createActions();
    void createMenu();
  
    QMenu *m_fileMenu;
    QMenu *m_editMenu;
    QAction *m_openDataFileAct;
    QAction *m_openMapFileAct;
    QAction *m_loadReferenceAct;
    QAction *m_buildMapAct;
    QString m_lastOpenDir;
};

#endif // PEDVIEWERMENUBAR_H
