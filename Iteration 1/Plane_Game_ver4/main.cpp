#include "gui_app.h"
#include "plane.h"
#include "bullet.h"
#include "enemy.h"
#include "explosion.h"
#include "factory.h"
#include "musicplay.h"
#include "reward.h"
#include <QPainter>
#include <QString>
#include <QTimer>
#include <QtWidgets>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //四个计时器，用于控制关卡难度，单位ms
    int t_Draw, t_Bullet, t_Enemy, t_Reward;
    //大、中、小敌机消灭的对应得分
    int Score_BigEnemy, Score_MidEnemy, Score_SmallEnemy;
    //初始玩家拥有的炸弹数
    int Init_BoomCount;
    //键盘控制飞机的灵敏度
    int Plane_Key_Speed;
    //胜利的分数条件，若取Win_Condition = -1 ，则为无限模式
    int Win_Condition;

    Score_BigEnemy = 25000;
    Score_MidEnemy = 6000;
    Score_SmallEnemy = 1000;
    Init_BoomCount = 5;
    Plane_Key_Speed = 5;
    Win_Condition = 100000;
    t_Draw = 20;
    t_Bullet = 200;
    t_Enemy = 500;
    t_Reward = 5000;


    gui_app Widget(t_Draw, t_Bullet, t_Enemy, t_Reward,
                   Score_BigEnemy, Score_MidEnemy, Score_SmallEnemy,
                   Init_BoomCount, Plane_Key_Speed, Win_Condition);
    Widget.setFixedSize(480,800);
    Widget.show();

    return a.exec();
}
