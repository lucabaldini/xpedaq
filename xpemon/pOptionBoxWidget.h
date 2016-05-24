#ifndef OPTIONBOXWIDGET_H
#define OPTIONBOXWIDGET_H

#include <iostream>

#include <QLineEdit>
#include <QGridLayout>
#include <QWidget>

#include "pQtCustomTextLabel.h"
#include "pQtGroupBoxWidget.h"

class pOptionBoxWidget: public pQtGroupBoxWidget
{
  public:
    explicit pOptionBoxWidget(QWidget *parent=0);
  
  private:
   pQtCustomTextLabel *m_socketPortLabel;
   QLineEdit *m_socketPortEdit;
   pQtCustomTextLabel *m_refreshTimeLabel;
   QLineEdit *m_refreshTimeEdit;
   pQtCustomTextLabel *m_zeroSupThrLabel;
   QLineEdit *m_zeroSupThrEdit;
};

#endif // OPTIONBOXWIDGET_H
