#include "pOptionBoxWidget.h"

pOptionBoxWidget::pOptionBoxWidget(QWidget *parent): pQtGroupBoxWidget(parent)
{
  m_socketPortLabel = new pQtCustomTextLabel(this, "Socket Port");
  m_socketPortEdit = new QLineEdit();
  m_refreshIntervalLabel = new pQtCustomTextLabel(this, "Refresh interval (ms)");
  m_refreshIntervalEdit = new QLineEdit();
  m_zeroSupThrLabel = new pQtCustomTextLabel(this, "Zero suppression");
  m_zeroSupThrEdit = new QLineEdit();
  addWidget(m_socketPortLabel, 0,0);
  addWidget(m_socketPortEdit, 0,1);
  addWidget(m_refreshIntervalLabel, 1,0);
  addWidget(m_refreshIntervalEdit, 1,1);
  addWidget(m_zeroSupThrLabel, 2,0);
  addWidget(m_zeroSupThrEdit, 2,1);
  initalizeText();
}


void pOptionBoxWidget::initalizeText()
{
  //TODO: the default value should be written somewhere else
  m_socketPortEdit -> setText("50001");
  m_refreshIntervalEdit -> setText("500");
  m_zeroSupThrEdit -> setText("6");
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
  //TO DO: add checks!
  socketPort = (m_socketPortEdit -> text()).toUInt();
  refreshInterval = (m_refreshIntervalEdit -> text()).toDouble();
  zeroSupThreshold = (m_zeroSupThrEdit -> text()).toUInt();
}
