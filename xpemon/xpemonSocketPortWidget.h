#ifndef SOCKETPORTWIDGET_H
#define SOCKETPORTWIDGET_H

#include <iostream>

#include<QLabel>
#include<QLineEdit>
#include <QGridLayout>
#include <QWidget>


class xpemonSocketPortWidget: public QWidget
{
  public:
    explicit xpemonSocketPortWidget(QWidget *parent=0);
  
  private:
   QLabel *m_socketPortLabel;
   QLineEdit *m_socketPortEdit;
   QGridLayout *m_socketPortLayout;
};

#endif // SOCKETPORTWIDGET_H
