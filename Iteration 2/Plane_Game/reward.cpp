#include "reward.h"
#include "entity.h"
#include <QString>
#include <QPainter>

Reward::Reward()
{
    this->setName("");
    this->setX(0);
    this->setY(0);
    this->setWidth(0);
    this->setHeight(0);
    this->setSpeedX(0);
    this->setSpeedY(0);
    this->setImage(QImage());
    this->StnTimes = 0;
    this->Counter = 0;
}

Reward::Reward(QString Name, int X, int Y, int Width, int Height, int SpeedX, int SpeedY, int StnTimes, QImage Image)
{
    this->setName(Name);
    this->setX(X);
    this->setY(Y);
    this->setWidth(Width);
    this->setHeight(Height);
    this->setSpeedX(SpeedX);
    this->setSpeedY(SpeedY);
    this->setImage(Image);
    this->StnTimes = StnTimes;
    this->Counter = 0;
}

int Reward::getStnTimes()
{
    return StnTimes;
}

int Reward::getCounter()
{
    return Counter;
}

void Reward::setStnTimes(int stntimes)
{
    this->StnTimes = stntimes;

}

void Reward::setCounter(int counter)
{
    Q_UNUSED(counter);
    this->Counter = 0;
}
