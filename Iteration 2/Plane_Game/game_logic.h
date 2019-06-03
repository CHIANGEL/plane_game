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

/*
 * 游戏逻辑类
 */

class Game_Logic
{
public:
    Game_Logic(int big, int mid, int small, int Key_Speed);
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

    void AI_Approach();  //游戏智能：敌机向玩家飞机移动
    void AI_Avoid();  //游戏智能：中、小型敌机躲闪玩家子弹,大型敌机硬抗

    void RecordStatus(QString filename, bool UseBoom);  //记录一次当前游戏状态
    void SaveRecord(QString srcPath, QString desPath);  //保存该局游戏记录
    void SaveArchive(QString filename);  //存档游戏
    void LoadArchive(QString filename);  //读档游戏

    int getPlaneX();
    int getPlaneY();
    int getPlaneWidth();
    int getPlaneHeight();
    QImage getPlaneImage();
    void setPlaneX(int x);
    void setPlaney(int y);

    int get_BulletList_Size();
    QString getBulletListName(int i);
    int getBulletListX(int i);
    int getBulletListY(int i);
    int getBulletListWidth(int i);
    int getBulletListHeight(int i);
    QImage getBulletListImage(int i);
    void BulletListPush(Bullet New);
    void BulletListClear();

    int get_EnemyList_Size();
    QString getEnemyListName(int i);
    int getEnemyListX(int i);
    int getEnemyListY(int i);
    int getEnemyListWidth(int i);
    int getEnemyListHeight(int i);
    QImage getEnemyListImage(int i);
    void EnemyListPush(Enemy New);
    void EnemyListClear();

    int get_RewardList_Size();
    QString getRewardListName(int i);
    int getRewardListX(int i);
    int getRewardListY(int i);
    int getRewardListWidth(int i);
    int getRewardListHeight(int i);
    QImage getRewardListImage(int i);
    void RewardListPush(Reward New);
    void RewardListClear();

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

    int getBestScore();
    int getWinCondition();
    int getScore();
    int getBoomCount();
    int getInitBoomCount();
    int getGameDiffi();
    int getDSTime();
    int getScoreBig();
    int getScoreMid();
    int getScoreSmall();
    int getKeySpeed();
    bool is_GameOver();
    bool is_Pause();
    bool is_Win();
    bool is_Start();
    bool is_Music_On();
    void GameStart();
    void setWinCondition(int winconditon);
    void setInitBoomCount(int InitBoomCount);
    void setBoomCount(int BoomCount);
    void setGameDiffi(int Diffi);
    void setMusicOn(bool on_off);
    void setScore(int Score);
    void setDoubleShootTime(int DST);
    void UpdateBestScore();

    void Review_UseBoom();

private:
    const int Score_BigEnemy;
    const int Score_MidEnemy;
    const int Score_SmallEnemy;
    const int Plane_Key_Speed;

    Plane plane;  //玩家飞机
    QList<Enemy> enemy_list;  //敌方飞机存储列表
    QList<Bullet> bullet_list;  //子弹存储列表
    QList<Explosion> explosion_list;  //爆炸效果存储列表
    QList<Reward> reward_list;  //奖励道具存储列表
    int Win_Condition; //胜利条件，-1为无限模式
    int score;  //玩家当前得分
    int Init_BoomCount;  //玩家初始炸弹数量
    int boom_count;  //玩家当前拥有的炸弹数量
    int double_bullet_stnTime;  //双弹时间
    bool pause;  //表示当前是否处于暂停状态
    bool b_GameOver;  //表示一局游戏是否结束
    bool b_Win; //表示一局游戏是否胜利
    bool b_GameStart;  //记录游戏是否开始
    bool b_Music_On;  //记录音乐是否播放
    int GameDiffi; //记录游戏难度。0,1,2分别对应简单、困难、噩梦
    int BestScore;  //记录玩家过去的最佳成绩
};

#endif // GAME_LOGIC_H
