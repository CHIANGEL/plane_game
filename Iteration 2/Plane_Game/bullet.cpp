#include "bullet.h"
#include "entity.h"
#include <QString>
#include <QPainter>

Bullet::Bullet(QString Name, int X, int Y, int Width, int Height, int SpeedX, int SpeedY, QImage Image)
{
    this->setName(Name);
    this->setX(X);
    this->setY(Y);
    this->setWidth(Width);
    this->setHeight(Height);
    this->setSpeedX(SpeedX);
    this->setSpeedY(SpeedY);
    this->setImage(Image);
}

Bullet::Bullet()
{
    this->setName("");
    this->setX(0);
    this->setY(0);
    this->setWidth(0);
    this->setHeight(0);
    this->setSpeedX(0);
    this->setSpeedY(0);
    this->setImage(QImage());
}
