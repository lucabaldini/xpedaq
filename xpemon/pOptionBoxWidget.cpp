#include "pOptionBoxWidget.h"

pOptionBoxWidget::pOptionBoxWidget(QWidget *parent): pQtGroupBoxWidget(parent)
{
  m_socketPortLabel = new pQtCustomTextLabel(this, "Socket Port");
  m_socketPortEdit = new QLineEdit();
  m_refreshTimeLabel = new pQtCustomTextLabel(this, "Refresh interval");
  m_refreshTimeEdit = new QLineEdit();
  m_zeroSupThrLabel = new pQtCustomTextLabel(this, "Zero suppression");
  m_zeroSupThrEdit = new QLineEdit();
  addWidget(m_socketPortLabel, 0,0);
  addWidget(m_socketPortEdit, 0,1);
  addWidget(m_refreshTimeLabel, 1,0);
  addWidget(m_refreshTimeEdit, 1,1);
  addWidget(m_zeroSupThrLabel, 2,0);
  addWidget(m_zeroSupThrEdit, 2,1);
}

