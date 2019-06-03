#ifndef GUI_APP_H
#define GUI_APP_H

#include "game_logic.h"
#include "musicplay.h"
#include "mybutton.h"
#include "review_app.h"
#include <QTimer>
#include <QRadioButton>
#include <QButtonGroup>
#include <QWidget>
#include <QFileDialog>
#include <QCloseEvent>

/*
 * 游戏界面类，继承自QWidget
 */

class Game_Logic;

class gui_app : public QWidget
{
    Q_OBJECT

public:
    gui_app(int tdraw, int Score_BigEnemy, int Score_MidEnemy, int Score_SmallEnemy, int Plane_Key_Speed,
            QWidget *parent = 0, Qt::WindowFlags flags = 0);
    ~gui_app();

public slots:
    void TimeDraw();
    void TimeBullet();
    void TimeEnemy();
    void TimeReward();
    void StartButtonPress();
    void LoadButtonPress();
    void ReviewButtonPress();
    void MusicButtonPress();

protected:
    void paintEvent(QPaintEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void keyPressEvent(QKeyEvent *e);
    void closeEvent(QCloseEvent *e);

private:
    const int t_draw_interval;
    int t_bullet_interval;
    int t_enemy_interval;
    int t_reward_interval;

    void StartInterface(); //开始游戏界面
    void Init();  //开始游戏的初始化操作
    QString MakeComment(bool is_win, int score, int bestscore, int win_condition);  //机器评价

    gui_app *Widget;

    MyButton *StartButton;  //开始游戏按钮
    MyButton *LoadButton;  //读档按钮
    MyButton *ReviewButton;  //回放按钮
    MyButton *MusicButton; //音乐播放按钮

    QRadioButton* InfiniteModel, *VictoryModel;  //单选按钮用于选择游戏模式和难度
    QRadioButton* EasyDiffi, *HardDiffi, *MareDiffi;
    QButtonGroup* GroupModel;
    QButtonGroup* GroupDiffi;

    QTimer t_Draw;  //四个计时器，分别控制绘图、子弹产生、敌机产生、道具产生的频率(难度）
    QTimer t_Bullet;
    QTimer t_Enemy;
    QTimer t_Reward;

    Game_Logic *Game;
    MusicPlay *music_shoot;
    MusicPlay *music_bgm;

};

#endif // GUI_APP_H
