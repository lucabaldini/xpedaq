#include "pOptionBoxWidget.h"

pOptionBoxWidget::pOptionBoxWidget(QWidget *parent): pQtGroupBoxWidget(parent)
{
  m_socketPortLabel = new pQtCustomTextLabel(this, "Socket Port");
  m_socketPortEdit = new QLineEdit();
  m_refreshIntervalLabel = new pQtCustomTextLabel(this,
                                                      "Refresh interval (ms)");
  m_refreshIntervalEdit = new QLineEdit();
  m_zeroSupThrLabel = new pQtCustomTextLabel(this, "Zero suppression");
  m_zeroSupThrEdit = new QLineEdit();
  addWidget(m_socketPortLabel, 0,0);
  addWidget(m_socketPortEdit, 0,1);
  addWidget(m_refreshIntervalLabel, 1,0);
  addWidget(m_refreshIntervalEdit, 1,1);
  addWidget(m_zeroSupThrLabel, 2,0);
  addWidget(m_zeroSupThrEdit, 2,1);
  
  // TODO: the default value should be written elsewhere
  m_socketPortText.setNum(50001);
  m_refreshIntervalText.setNum(500.);
  m_zeroSupThresholdText.setNum(1);
  initalizeText();
}


void pOptionBoxWidget::initalizeText()
{
  m_socketPortEdit -> setText(m_socketPortText);
  m_refreshIntervalEdit -> setText(m_refreshIntervalText);
  m_zeroSupThrEdit -> setText(m_zeroSupThresholdText);
}


void pOptionBoxWidget::activateWidgets()
{
  m_socketPortEdit -> setDisabled(false);
  m_refreshIntervalEdit -> setDisabled(false);
  m_zeroSupThrEdit -> setDisabled(false);  
}


void pOptionBoxWidget::disableWidgets()
{
  m_socketPortEdit -> setDisabled(true);
  m_refreshIntervalEdit -> setDisabled(true);
  m_zeroSupThrEdit -> setDisabled(true);  
}


void pOptionBoxWidget::options(unsigned int &socketPort,
                               double &refreshInterval,
                               unsigned int  &zeroSupThreshold)
{ 
  /* Read the options inserted by the user in the option boxes.
     If an option is invalid restore the last valid value inserted */
  readSocketPort(socketPort);
  readRefreshInterval(refreshInterval);
  readZeroSupThreshold(zeroSupThreshold);
}


void pOptionBoxWidget::readSocketPort(unsigned int &socketPort)
{
  bool convSuccess;
  socketPort = (m_socketPortEdit -> text()).toUInt(&convSuccess);
  if (!convSuccess)
  {
    socketPort = m_socketPortText.toUInt();
    m_socketPortEdit -> setText(m_socketPortText);
    return;
  }
  if (socketPort > 65535)  // maximum value for Udp socket port
  {
    socketPort = m_socketPortText.toUInt();
    m_socketPortEdit -> setText(m_socketPortText);
    return;
  }
  m_socketPortText = m_socketPortEdit -> text();
}


void pOptionBoxWidget::readRefreshInterval(double &refreshInterval)
{
  bool convSuccess;
  refreshInterval = (m_refreshIntervalEdit -> text()).toDouble(&convSuccess);
  if (!convSuccess)
  {
    refreshInterval = m_refreshIntervalText.toDouble();
    m_refreshIntervalEdit -> setText(m_refreshIntervalText);
    return;
  }
  if (refreshInterval <= 0.)
  {
    refreshInterval = m_refreshIntervalText.toDouble();
    m_refreshIntervalEdit -> setText(m_refreshIntervalText);
    return;
  }
  m_refreshIntervalText = m_refreshIntervalEdit -> text();
}


void pOptionBoxWidget::readZeroSupThreshold(unsigned int &zeroSupThreshold)
{
  bool convSuccess;
  zeroSupThreshold = (m_zeroSupThrEdit -> text()).toUInt(&convSuccess);
  if (!convSuccess)
  {
    zeroSupThreshold = m_zeroSupThresholdText.toUInt();;
    m_zeroSupThrEdit -> setText(m_zeroSupThresholdText);
  }
  m_zeroSupThresholdText = m_zeroSupThrEdit -> text();
}
