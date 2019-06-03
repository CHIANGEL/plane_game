#include "entity.h"
#include <QString>
#include <QPainter>

QString Entity::getName()
{
    return Name;
}

int Entity::getX()
{
    return X;
}

int Entity::getY()
{
    return Y;
}

int Entity::getWidth()
{
    return Width;
}

int Entity::getHeight()
{
    return Height;
}

int Entity::getSpeedX()
{
    return SpeedX;
}

int Entity::getSpeedY()
{
    return SpeedY;
}

QImage Entity::getImage()
{
    return Image;
}

void Entity::setName(QString Name)
{
    this->Name = Name;
}

void Entity::setX(int X)
{
    this->X = X;
}

void Entity::setY(int Y)
{
    this->Y = Y;
}

void Entity::setWidth(int Width)
{
    this->Width = Width;
}

void Entity::setHeight(int Height)
{
    this->Height = Height;
}

void Entity::setSpeedX(int SpeedX)
{
    this->SpeedX = SpeedX;
}

void Entity::setSpeedY(int SpeedY)
{
    this->SpeedY = SpeedY;
}

void Entity::setImage(QImage Image)
{
    this->Image = Image;
}
