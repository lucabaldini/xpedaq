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


#ifndef HISTOGRAMPLOT_H
#define HISTOGRAMPLOT_H

#include <QMutex>

#include "pCustomHistogramPlot.h"
#include "pHistogram.h"
#include "pHistogramOptions.h"

#define TOLERANCE_FACTOR  1.e-5


/* Class for drawing a 1d histogram. Inherits from pCustomHistogramPlot.
   The underlying data structure is a pHistogram object, which is
   passed as a constant to the constructor and cannot be modified in any way
   (nor deleted) inside the class.
   The class is responsible for synchronizing what is displayed on the screen
   with the content of the histogram.
*/
   
class pHistogramPlot: public pCustomHistogramPlot
{
  
  Q_OBJECT
  
  public:
       
    //Initialize from existing pHistogram
    pHistogramPlot(const pHistogram* hist, 
                   pBasicPlotOptions options = pBasicPlotOptions());    
    unsigned int entries() const;
    double sum() const;
    void reset();

  public slots:

    /* Reset the x-axis to match the histogram dimension and set the
      optimal y-axis scale. */
    virtual void resetView();
    
    /* Synchronize the plot with the content of the pHistogram */
    void updateDisplay();
      
  private:
    
    pHistogramPlot(){;} //so it cannot be used
    const pHistogram *m_hist;
    
};

#endif  //HISTOGRAMPLOT_H
