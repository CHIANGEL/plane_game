#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#include "gui_app.h"
#include "plane.h"
#include "bullet.h"
#include "enemy.h"
#include "explosion.h"
#include "factory.h"
#include "reward.h"
#include <QPainter>
#include <QString>
#include <QTimer>
#include <QtWidgets>

class Game_Logic
{
    //friend class gui_app;

public:
    Game_Logic(int big, int mid, int small, int BCount, int Key_Speed, int win_condition);
    void Init();

    void Shoot_Bullet(int t_draw_interval);  //发射子弹
    void Enemy_Gene(int WidgetWidth);  //产生新的敌机
    void Reward_Gene(int WidgetWidth);  //产生新的道具

    void is_Enemy_Down();  //敌机消灭检测
    void is_Reward_Get();  //道具获取检测
    bool is_Plane_Down();  //玩家飞机碰撞检测(游戏结束条件)

    int Mouse_Move(int x, int y);
    int Mouse_Press(int x, int y, Qt::MouseButton button);
    int Key_Press(Qt::Key key);

    void Move_Bullet();  //移动子弹
    void Move_Enemy(int Widgetwidth, int WidgetHeight);  //移动敌机
    void Move_Reward(int WidgetHeight);  //移动道具

    int getPlaneX();
    int getPlaneY();
    int getPlaneWidth();
    int getPlaneHeight();
    QImage getPlaneImage();

    int get_BulletList_Size();
    int getBulletListX(int i);
    int getBulletListY(int i);
    int getBulletListWidth(int i);
    int getBulletListHeight(int i);
    QImage getBulletListImage(int i);

    int get_EnemyList_Size();
    int getEnemyListX(int i);
    int getEnemyListY(int i);
    int getEnemyListWidth(int i);
    int getEnemyListHeight(int i);
    QImage getEnemyListImage(int i);

    int get_RewardList_Size();
    int getRewardListX(int i);
    int getRewardListY(int i);
    int getRewardListWidth(int i);
    int getRewardListHeight(int i);
    QImage getRewardListImage(int i);

    int get_ExplosionList_Size();
    int getExplosionListX(int i);
    int getExplosionListY(int i);
    int getExplosionListWidth(int i);
    int getExplosionListHeight(int i);
    int getExplosionListCounter(int i);
    int getExplosionListStnTimes(int i);
    int get_ExplosionList_ImagesSize(int i);
    QImage getExplosionListImage(int i, int j);
    void setExplosionListCounter(int i, int counter);
    void removeExplosionList(int i);

    int getWinCondition();
    int getScore();
    int getBoomCount();
    bool is_GameOver();
    bool is_Pause();
    bool is_Win();


private:
    const int Score_BigEnemy;
    const int Score_MidEnemy;
    const int Score_SmallEnemy;
    const int Init_BoomCount;
    const int Plane_Key_Speed;
    const int Win_Condition;

    Plane plane;  //玩家飞机
    QList<Enemy> enemy_list;  //敌方飞机存储列表
    QList<Bullet> bullet_list;  //子弹存储列表
    QList<Explosion> explosion_list;  //爆炸效果存储列表
    QList<Reward> reward_list;  //奖励道具存储列表
    int score;  //玩家当前得分
    int boom_count;  //玩家当前拥有的炸弹数量
    int double_bullet_stnTime;  //双弹时间
    bool pause;  //表示当前是否处于暂停状态
    bool b_GameOver;  //表示一局游戏是否结束
    bool b_Win; //表示一局游戏是否胜利

};

#endif // GAME_LOGIC_H
