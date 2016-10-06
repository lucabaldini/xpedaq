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

    //int run(const pEvent event);
    //void run(event);
    //void run(event, threshold);
    //void run(event, threshold, pivot);
    //void run(event, threshold, pivot, weights)
    
    // Getters
    double x0() const {return m_x0;}
    double y0() const {return m_y0;}
    double phi() const {return m_phi;}
    double mom2long() const {return m_mom2Long;}        
    double mom2trans() const {return m_mom2Trans;}
    
    // Setters
    void setX0(double x0) {m_x0 = x0;}
    void setY0(double y0) {m_y0 = y0;}
    void setPhi(double phi) {m_phi = phi;}
    void setMom2long(double mom2long) {m_mom2Long = mom2long;}
    void setMom2trans(double mom2trans) {m_mom2Trans = mom2trans;}
    
    // Terminal formatting.
    std::ostream& fillStream(std::ostream& os) const;
    friend std::ostream& operator<<(std::ostream& os,
				    const pMomentsAnalysis& moma)
    {return moma.fillStream(os);}
  
  private:

    /*! \brief The x coordinate of the pivot point.
     */
    double m_x0;

    /*! \brief The y coordinate of the pivot point.
     */
    double m_y0;
    
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
