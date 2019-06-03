#ifndef MYBUTTON_H
#define MYBUTTON_H

#include <QPushButton>
#include <QString>

/*
 * 按钮类，继承自Qt内置类QPushbutton
 */

class MyButton : public QPushButton
{
public:
    MyButton(QString Name, QWidget *parent = 0);
};

#endif // MYBUTTON_H
