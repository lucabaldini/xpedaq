#ifndef OPTIONBOXWIDGET_H
#define OPTIONBOXWIDGET_H

#include <QLineEdit>
#include <QGridLayout>
#include <QWidget>
#include <QString>

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
    void readSocketPort(unsigned int &socketPort);
    void readRefreshInterval(double &refreshInterval);
    void readZeroSupThreshold(unsigned int &zeroSupThreshold);
  
    pQtCustomTextLabel *m_socketPortLabel;
    QLineEdit *m_socketPortEdit;
    pQtCustomTextLabel *m_refreshIntervalLabel;
    QLineEdit *m_refreshIntervalEdit;
    pQtCustomTextLabel *m_zeroSupThrLabel;
    QLineEdit *m_zeroSupThrEdit;    
    QString m_socketPortText;
    QString m_refreshIntervalText;
    QString m_zeroSupThresholdText;
};

#endif // OPTIONBOXWIDGET_H
