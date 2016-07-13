#include "pInfoBoxWidget.h"

pInfoBoxWidget::pInfoBoxWidget(QWidget *parent): pQtGroupBoxWidget(parent)
{
  m_evtCounter = 0;
  
  m_nEventsLabel = new pQtCustomTextLabel(this, "n. Events");
  m_nEventsEdit = new QLineEdit();
  m_nEventsEdit -> setReadOnly(true);
  
  m_windowSizeLabel = new pQtCustomTextLabel(this, "Window Size");
  m_windowSizeEdit = new QLineEdit();
  m_windowSizeEdit -> setReadOnly(true);
  
  m_highestPixelLabel = new pQtCustomTextLabel(this, "Max position");
  m_highestXEdit = new QLineEdit();
  m_highestYEdit = new QLineEdit();
  m_highestXEdit -> setReadOnly(true);
  m_highestYEdit -> setReadOnly(true);
  
  m_barycenterLabel = new pQtCustomTextLabel(this,
                                             "Barycenter position");
  m_barycenterXEdit = new QLineEdit();
  m_barycenterYEdit = new QLineEdit();
  m_barycenterXEdit -> setReadOnly(true);
  m_barycenterYEdit -> setReadOnly(true);
  
  addWidget(m_nEventsLabel, 0, 0);
  addWidget(m_nEventsEdit, 0, 1);
  addWidget(m_windowSizeLabel, 1, 0);
  addWidget(m_windowSizeEdit, 1, 1);
  addWidget(m_highestPixelLabel, 2, 0);
  addWidget(m_highestXEdit, 3, 0);
  addWidget(m_highestYEdit, 3, 1);
  
  // Add some vertical space
  int row = m_groupBoxGridLayout -> rowCount();
  int verticalPixelSpace  = 15;
  m_groupBoxGridLayout -> setRowMinimumHeight(row, verticalPixelSpace);
  
  addWidget(m_barycenterLabel, 5, 0);
  addWidget(m_barycenterXEdit, 6, 0);
  addWidget(m_barycenterYEdit, 6, 1);
  
  initalizeText();
}


void pInfoBoxWidget::initalizeText()
{
  m_nEventsEdit -> setText("0");
  m_windowSizeEdit -> setText("0");
  m_highestXEdit -> setText("0");
  m_highestYEdit -> setText("0");
  m_barycenterXEdit -> setText("0");
  m_barycenterYEdit -> setText("0");
}


void pInfoBoxWidget::updateCounter()
{
  m_evtCounter += 1;
  m_nEventsEdit -> setText(QString::number(m_evtCounter));
}


void pInfoBoxWidget::updateWindowSize(unsigned int xmin, unsigned int xmax,
                                      unsigned int ymin, unsigned int ymax)
{
  unsigned int windowSize = (xmax - xmin + 1) * (ymax - ymin + 1);
  m_windowSizeEdit -> setText(QString::number(windowSize));
}


void pInfoBoxWidget::updateMaxCoordinates(unsigned int x, unsigned int y)
{
  m_highestXEdit -> setText(QString::number(x));
  m_highestYEdit -> setText(QString::number(y));
}


void pInfoBoxWidget::updateBarycenterCoordinates(double x, double y)
{
  m_barycenterXEdit -> setText(QString::number(x, 'f', 2));
  m_barycenterYEdit -> setText(QString::number(y, 'f', 2));
}


void pInfoBoxWidget::reset()
{
  m_evtCounter = 0;
  initalizeText();
}
