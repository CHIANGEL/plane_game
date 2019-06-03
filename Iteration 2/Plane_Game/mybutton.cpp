#include "mybutton.h"

MyButton::MyButton(QString Name, QWidget *parent)
    : QPushButton(parent)
{
    this->setFont(QFont("幼圆",14));
    this->setStyleSheet("background-color:rgb(250, 250, 250); color:black; border-radius:10px; border:2px groove gray; border-style:outset;");
    this->setText(Name);
}
