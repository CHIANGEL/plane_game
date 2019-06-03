#ifndef PLANE_H
#define PLANE_H

#include "entity.h"
#include <QPainter>
#include <QString>

/*
 * 玩家飞机类，继承自实体类
 */

class Plane : public Entity
{
public:
    Plane(QString Name, int X, int Y, int Width, int Height, int SpeedX,int SpeedY, QImage Image);
    Plane();

private:

};

#endif // PLANE_H
