#ifndef FACTORY_H
#define FACTORY_H

#include "bullet.h"
#include "enemy.h"
#include "plane.h"
#include "reward.h"
#include "explosion.h"
#include <QPainter>

/*
 * 工厂类，用于构建各类实体
 */

class Factory
{
public:
    static Plane GenPlane();
    static Enemy GenEnemy(QString kind,int speedBase,int width,int GameDiffi);
    static Enemy GenEnemy(QString kind, int x, int y ,int speedx, int speedy, int hp); //重载敌机生产函数
    static Bullet GenBullet(QString kind,int X,int Y);
    static Reward GenReward(QString kind, int X, int Y);
    static Reward GenReward(QString kind, int x, int y, int speedx, int speedy, int stntimes);  //重载道具生产函数
    static QImage GetBoomIcon();  //获取炸弹图标
    static Explosion GenExplosion(QString kind, int X, int Y);
};

#endif // FACTORY_H
