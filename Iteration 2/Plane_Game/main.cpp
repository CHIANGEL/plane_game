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

    //刷新绘图的速度，单位ms
    int t_Draw;
    //大、中、小敌机消灭的对应得分
    int Score_BigEnemy, Score_MidEnemy, Score_SmallEnemy;
    //键盘控制飞机的灵敏度
    int Plane_Key_Speed;

    Score_BigEnemy = 25000;
    Score_MidEnemy = 6000;
    Score_SmallEnemy = 1000;
    Plane_Key_Speed = 20;
    t_Draw = 20;


    gui_app Widget(t_Draw, Score_BigEnemy, Score_MidEnemy, Score_SmallEnemy, Plane_Key_Speed);
    Widget.setFixedSize(480,800);
    Widget.show();

    return a.exec();
}
