#include "pCommentBoxWidget.h"

pCommentBoxWidget::pCommentBoxWidget(QWidget *parent, std::string comment):
                                     pQtGroupBoxWidget(parent)
{
  setTitle("User comment");
  m_commentEdit = new QTextEdit();
  addWidget(m_commentEdit, 0,0);
  
  // Display the initial value
  m_commentEdit -> setText(QString::fromStdString(comment));
}


void pCommentBoxWidget::activate()
{
  m_commentEdit -> setDisabled(false);
}


void pCommentBoxWidget::disable()
{
  m_commentEdit -> setDisabled(true);
}


std::string pCommentBoxWidget::comment() const
{ 
  return (m_commentEdit -> toPlainText()).toStdString();
}

void pCommentBoxWidget::setText(std::string usrText)
{
  m_commentEdit -> setText(QString::fromStdString(usrText));
}


void pCommentBoxWidget::reset()
{
  m_commentEdit -> setText("");
}
