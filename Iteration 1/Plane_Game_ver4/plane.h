#ifndef PLANE_H
#define PLANE_H

#include "entity.h"
#include <QPainter>
#include <QString>

/*
玩家飞机类，继承自实体类
*/

class Plane : public Entity
{
public:
    Plane(QString Name, int X, int Y, int Width, int Height, int SpeedX,int SpeedY, QImage Image);
    Plane();
    int getLevel();
    void LevelUp();  //飞机升级

private:
    int level;  //飞机等级

};

#endif // PLANE_H
