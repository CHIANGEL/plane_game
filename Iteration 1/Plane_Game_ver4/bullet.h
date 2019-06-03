#ifndef BULLET_H
#define BULLET_H

#include "entity.h"
#include <QPainter>
#include <QString>

/*
子弹类，继承自实体类
*/

class Bullet : public Entity
{
public:
    Bullet(QString Name, int X, int Y, int Width, int Height, int SpeedX,int SpeedY, QImage Image);
    Bullet();
};

#endif // BULLET_H
