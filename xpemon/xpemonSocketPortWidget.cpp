#include "xpemonSocketPortWidget.h"

xpemonSocketPortWidget::xpemonSocketPortWidget(QWidget *parent):
                                                            QWidget(parent)
{
  m_socketPortLayout = new QGridLayout(this);
  m_socketPortLabel = new QLabel("Socket Port");
  m_socketPortEdit = new QLineEdit();
  m_socketPortLayout->addWidget(m_socketPortLabel, 0,0);
  m_socketPortLayout->addWidget(m_socketPortEdit, 0,1);
}
