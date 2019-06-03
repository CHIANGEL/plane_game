#ifndef GUI_APP_H
#define GUI_APP_H

#include "game_logic.h"
#include "musicplay.h"
#include <QTimer>
#include <QWidget>

class Game_Logic;

class gui_app : public QWidget
{
    Q_OBJECT

public:
    gui_app(int tdraw, int tbullet, int tenemy, int treward,
            int Score_BigEnemy, int Score_MidEnemy, int Score_SmallEnemy,
            int Init_BoomCount, int Plane_Key_Speed, int win_condition,
            QWidget *parent = 0, Qt::WindowFlags flags = 0);
    ~gui_app();

public slots:
    void TimeDraw();
    void TimeBullet();
    void TimeEnemy();
    void TimeReward();

protected:
    void paintEvent(QPaintEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void keyPressEvent(QKeyEvent *e);

private:
    const int t_draw_interval;
    const int t_bullet_interval;
    const int t_enemy_interval;
    const int t_reward_interval;

    void Init();

    QTimer t_Draw;  //四个计时器，分别控制绘图、子弹产生、敌机产生、道具产生的频率(难度）
    QTimer t_Bullet;
    QTimer t_Enemy;
    QTimer t_Reward;

    Game_Logic *Game;
    MusicPlay *music_shoot;
    MusicPlay *music_bgm;

};

#endif // GUI_APP_H
