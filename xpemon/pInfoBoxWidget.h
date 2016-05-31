#ifndef INFOBOXWIDGET_H
#define INFOBOXWIDGET_H

#include <iostream>

#include <QLineEdit>
#include <QGridLayout>
#include <QWidget>

#include "pQtCustomTextLabel.h"
#include "pQtGroupBoxWidget.h"

class pInfoBoxWidget: public pQtGroupBoxWidget
{

  Q_OBJECT

  public:

    explicit pInfoBoxWidget(QWidget *parent=0);
  
  public slots:
  
    void updateCounter();
    void updateWindowSize(unsigned int xmin, unsigned int xmax,
                          unsigned int ymin, unsigned int ymax);
    void updateMaxCoordinates(unsigned int x, unsigned int y);
    void reset();
 
  private:
  
    void initalizeText();
  
    pQtCustomTextLabel *m_nEventsLabel;
    QLineEdit *m_nEventsEdit;
    pQtCustomTextLabel *m_windowSizeLabel;
    QLineEdit *m_windowSizeEdit;
    pQtCustomTextLabel *m_highestPixelLabel;
    QLineEdit *m_highestXEdit;
    QLineEdit *m_highestYEdit;
    
    unsigned int m_evtCounter;
};

#endif // INFBOXWIDGET_H
