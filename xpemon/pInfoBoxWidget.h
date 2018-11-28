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

#ifndef INFOBOXWIDGET_H
#define INFOBOXWIDGET_H

#include <sstream>
#include <iomanip>

#include <QLineEdit>
#include <QGridLayout>
#include <QWidget>
#include <QCheckBox>

#include "pQtCustomTextLabel.h"
#include "pInfoBoxGui.h"


class pInfoBoxWidget: public pInfoBoxGui
{

  Q_OBJECT

  public:

    explicit pInfoBoxWidget(QWidget *parent=0);

    // Getters
    QCheckBox *drawFirstPassCheckBox() {return m_drawFirstPassCheckBox;}
    QCheckBox *drawSearchRegionCheckBox() {return m_drawSearchRegionCheckBox;}
    QCheckBox *drawSecondPassCheckBox() {return m_drawSecondPassCheckBox;}
    QCheckBox *showRawEventsCheckBox() {return m_showRawEventsCheckBox;}

    void checkCheckBoxes(bool checked);

  public slots:
      
    void updateTime(double seconds);
    void updateAbsorptionPoint(double x, double y);
    void updatePulseHeight(int pulseHeight);
    void updatePhi(double phi);

    void updateWindowSize(unsigned int colMin, unsigned int colMax,
                          unsigned int rowMin, unsigned int rowMax);
    void updateMaxCoordinates(int col, int row);
    void updateBaricenterCoordinates(double x, double y);
    void updateClusterSize(int size);
    void updateMom2Trans(double mom2Trans);
    void updateMom2Long(double mom2Long);
    void updateMomRatio(double momRatio);
    void updateSkewness(double skewness);
    void setReconOptionsEnabled(int status);
    void reset();
 
  private:
  
    void initializeText();
    template<typename T>
    std::string number2String (T x, int precision = 4);
    template<typename T>
    std::string coordinateStringFormat(T x, T y, int precision = 4);

    QString m_timeLabelName;
    QString m_absorptionPointLabelName;
    QString m_pulseHeightLabelName;
    QString m_phiLabelName;
    
    QString m_windowSizeLabelName;
    QString m_maxPosLabelName;
    QString m_baricenterPosLabelName;
    QString m_clusterSizeLabelName;
    QString m_mom2LongLabelName;
    QString m_mom2TransLabelName;
    QString m_momRatioLabelName;
    QString m_skewnessLabelName;

    QCheckBox *m_drawFirstPassCheckBox;
    QCheckBox *m_drawSearchRegionCheckBox;
    QCheckBox *m_drawSecondPassCheckBox;
    QCheckBox *m_showRawEventsCheckBox;
};


// Definitions here because of template restricition
template<typename T>
std::string pInfoBoxWidget::number2String(T x, int precision)
{
  std::ostringstream ostr; //output string stream
  ostr << std::setprecision(precision) << x;
  return ostr.str();
}


template<typename T>
std::string pInfoBoxWidget::coordinateStringFormat(T x, T y, int precision)
{
  std::ostringstream ostr; //output string stream
  ostr << std::setprecision(precision) << x << ", " << y;
  return ostr.str();
}


#endif // INFBOXWIDGET_H
