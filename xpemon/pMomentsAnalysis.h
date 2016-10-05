/***********************************************************************
Copyright (C) 2016 the X-ray Polarimetry Explorer (XPE) team.

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

#ifndef PMOMENTS_ANALYSIS_H
#define PMOMENTS_ANALYSIS_H

#include <iostream>

/* Class describing the outcome of a moments analysis. */

class pMomentsAnalysis
{
  public:
  
    pMomentsAnalysis();

    int run();
    //void run(event);
    //void run(event, threshold);
    //void run(event, threshold, pivot);
    //void run(event, threshold, pivot, weights)
    
    // Terminal formatting.
    std::ostream& fillStream(std::ostream& os) const;
    friend std::ostream& operator<<(std::ostream& os,
				    const pMomentsAnalysis& moma)
    {return moma.fillStream(os);}
  
  private:

    /*! \brief The x coordinate of the pivot point.
     */
    double m_xPivot;

    /*! \brief The y coordinate of the pivot point.
     */
    double m_yPivot;
    
    /*! \brief The azimuthal angle of the major axis of the charge distribution.
     */
    double m_phi;

    /*! \brief The transverse second moment.
     */
    double m_mom2Trans;

    /*! \brief The longitudinal second moment.
     */
    double m_mom2Long;

    enum StatusCode
    {
      UNDEFINED,
      SUCCESS,
      FAILURE
    };

    /*! \brief Internal status code to store the outcome of the moments
      analysis.
     */
    int m_statusCode;
};

#endif //PMOMENTS_ANALYSIS_H
