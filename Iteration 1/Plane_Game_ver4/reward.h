#ifndef REWARD_H
#define REWARD_H

#include "entity.h"
#include <QPainter>
#include <QString>

/*
奖励道具类，包括炸弹和双弹两种。继承自Entity
*/

class Reward : public Entity
{
public:
    Reward(QString Name, int X, int Y, int Width, int Height, int SpeedX,int SpeedY, int StnTimes, QImage Image);
    Reward();
    int getStnTimes();
    int getCounter();
    void setStnTimes(int stntimes);
    void setCounter(int counter);

private:
    int StnTimes; //双弹持续时间
    int Counter;
};

#endif // REWARD_H
