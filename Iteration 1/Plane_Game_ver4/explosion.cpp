#include "explosion.h"

Explosion::Explosion(int X, int Y, int StnTimes, QList<QImage> Images)
{
    this->X = X;
    this->Y = Y;
    this->StnTimes = StnTimes;
    this->Images = Images;
    this->Counter = 0;
}

int Explosion::getX()
{
    return this->X;
}

int Explosion::getY()
{
    return this->Y;
}

int Explosion::getWidth()
{
    return this->Width;
}

int Explosion::getHeight()
{
    return this->Height;
}

int Explosion::getCounter()
{
    return this->Counter;
}

int Explosion::getStnTimes()
{
    return this->StnTimes;
}

int Explosion::getImageSize()
{
    return Images.size();
}

QImage Explosion::getImage(int i)
{
    return Images[i];
}

void Explosion::setCounter(int counter)
{
    this->Counter = counter;
}
