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

#include "pedRunController.h"


pedRunController::pedRunController(std::string configFilePath,
                                   std::string preferencesFilePath,
                                   std::string trgMaskFilePath,
                                   double nSigmaAlarmThreshold,
                                   std::string usrComment) : 
  pRunController(configFilePath, preferencesFilePath, trgMaskFilePath,
    usrComment, true),
  m_nSigmaAlarmThreshold(nSigmaAlarmThreshold)                                 
{
  m_pedestalMap = new PedestalsMap();
  connect (m_dataCollector, SIGNAL(blockRead(const pDataBlock&)),
          this, SLOT(readDataBlock(const pDataBlock&)));
}                                   

void pedRunController::readDataBlock(const pDataBlock &p)
{
  for (unsigned int evt = 0; evt < p.numEvents(); ++evt) {   
    unsigned int x = 1000; //unphysical initialization
    unsigned int y = 1000; //unphysical initialization
    adc_count_t height = 0;
    for (unsigned int index = 0; index < p.numPixels(evt); ++index) {
      p.readPixel(evt, index, x, y, height);
      double dist = m_pedestalMap->normDistance(x, y, height);
      if (dist > m_nSigmaAlarmThreshold){
        *xpollog::kError << "Outlier pixel at (" << x << "," << y << ")."
                         << " Value = " << height << ", Norm. distance = "
                         << dist << " [sigma]" << endline;
      }
      m_pedestalMap->fill(x, y, height);
    }
  }	
}


void pedRunController::resetPedMap()
{
  m_pedestalMap->reset();
}
