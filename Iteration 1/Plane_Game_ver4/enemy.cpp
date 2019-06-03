#include "enemy.h"
#include <QPainter>
#include <QString>

Enemy::Enemy(QString Name, int X, int Y, int Width, int Height, int SpeedX, int SpeedY, int HP, QImage Image)
{
    this->setName(Name);
    this->setX(X);
    this->setY(Y);
    this->setWidth(Width);
    this->setHeight(Height);
    this->setSpeedX(SpeedX);
    this->setSpeedY(SpeedY);
    this->setImage(Image);
    this->HP = HP;
}

void Enemy::HP_down()
{
    this->HP--;
}

int Enemy::getHP()
{
    return this->HP;
}
