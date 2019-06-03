#ifndef ENEMY_H
#define ENEMY_H

#include "entity.h"
#include <QPainter>
#include <QString>

/*
 * 敌方飞机类，继承自实体类
 */

class Enemy : public Entity
{
public:
    Enemy(QString Name, int X, int Y, int Width, int Height, int SpeedX, int SpeedY, int HP, QImage Image);
    void HP_down();
    int getHP();

private:
    int HP;

};

#endif // ENEMY_H
