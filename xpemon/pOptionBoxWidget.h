#ifndef OPTIONBOXWIDGET_H
#define OPTIONBOXWIDGET_H

#include <QLineEdit>
#include <QGridLayout>
#include <QWidget>

#include "pQtCustomTextLabel.h"
#include "pQtGroupBoxWidget.h"

class pOptionBoxWidget: public pQtGroupBoxWidget
{

  Q_OBJECT

  public:

    explicit pOptionBoxWidget(QWidget *parent=0);
    void options(unsigned int &socketPort, double &refreshInterval, 
                 unsigned int  &zeroSupThreshold);
  
  public slots:

    void activateWidgets();
    void disableWidgets();
  
  private:
  
    void initalizeText();
  
    pQtCustomTextLabel *m_socketPortLabel;
    QLineEdit *m_socketPortEdit;
    pQtCustomTextLabel *m_refreshIntervalLabel;
    QLineEdit *m_refreshIntervalEdit;
    pQtCustomTextLabel *m_zeroSupThrLabel;
    QLineEdit *m_zeroSupThrEdit;
};

#endif // OPTIONBOXWIDGET_H
