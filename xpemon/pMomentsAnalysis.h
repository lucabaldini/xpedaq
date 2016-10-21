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
#include <cmath>
#include <algorithm>

#include "xpoldetector.h"

#include "qcustomplot.h"
#include "pRotableEllipse.h"


/* Class describing the outcome of a moments analysis. */

class pMomentsAnalysis
{
  public:
  
    pMomentsAnalysis();

    // Run the moments analysis on a cluster (unweighted version).
    int run(const std::vector<event::Hit> &hits, double threshold,
	    double x0, double y0, int clusterId = 0);
    
    // Run the moments analysis on a cluster (weighted version).
    int run(const std::vector<event::Hit> &hits, double threshold,
	    double x0, double y0, const std::vector<double> &weights,
	    int clusterId = 0);
    
    // Getters
    double x0() const {return m_x0;}
    double y0() const {return m_y0;}
    double phi() const {return m_phi;}
    double phiDeg() const {return 180. * m_phi / M_PI;}
    double phiFolded() const;
    double phiFoldedDeg() const {return 180. * phiFolded() / M_PI;}
    double mom2long() const {return m_mom2long;}        
    double mom2trans() const {return m_mom2trans;}
    double rmsLong() const {return sqrt(m_mom2long);}
    double rmsTrans() const {return sqrt(m_mom2trans);}
    double mom3long() const {return m_mom3long;}
    double skewness() const;
    double elongation() const;

    //
    void rotatePhi(double angle) {m_phi += angle;}
    void flip();
    void flip3();
    
    // Terminal formatting.
    std::ostream& fillStream(std::ostream& os) const;
    friend std::ostream& operator<<(std::ostream& os,
				    const pMomentsAnalysis& moma)
    {return moma.fillStream(os);}
    
    // Draw on the event display
    void draw(QCustomPlot* parentPlot, const QColor &lineColor = "blue",
	      bool pivot = true, bool axis = true, bool ellipse = true,
	      int lineWidth = 0,
	      const Qt::PenStyle axisStyle = Qt::SolidLine,
	      const Qt::PenStyle ellipseStyle = Qt::SolidLine) const;
    // Drae the pivot of the moments analtsys.
    void drawPivot(QCustomPlot* parentPlot, const QColor &color) const;
    // Draw the principal axis.
    void drawPrincipalAxis(QCustomPlot* parentPlot, const QColor &color,
			   int lineWidth, const Qt::PenStyle style) const;
    // Draw the ellipsoid of inertia.
    void drawEllipse(QCustomPlot* parentPlot, const QColor &color,
		     int lineWidth, const Qt::PenStyle style) const;
  
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

    /*! \brief The longitudinal second moment.
     */
    double m_mom2long;

    /*! \brief The transverse second moment.
     */
    double m_mom2trans;
    
    /*! \brief
     */
    double m_mom3long;

    enum StatusCode
    {
      UNDEFINED,
      SUCCESS,
      NOT_ENOUGH_HITS
    };

    /*! \brief Internal status code to store the outcome of the moments
      analysis.
     */
    int m_statusCode;
};

#endif //PMOMENTS_ANALYSIS_H
