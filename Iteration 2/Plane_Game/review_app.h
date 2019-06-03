#ifndef REVIEW_APP_H
#define REVIEW_APP_H

#include "game_logic.h"
#include "musicplay.h"
#include "mybutton.h"
#include <QTimer>
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QRadioButton>
#include <QButtonGroup>
#include <QWidget>
#include <QCloseEvent>

/*
 * “精彩回放”类，用于回放上一局被保存的游戏记录，为独立于gui_app的一个窗口
 * 继承自 QWidget 类
 */

class Game_Logic;

class review_app : public QWidget
{
    Q_OBJECT

public:
    review_app(QString filename, int tdraw, int Score_BigEnemy, int Score_MidEnemy, int Score_SmallEnemy, QWidget *parent = 0, Qt::WindowFlags flags = 0);
    ~review_app();

public slots:
    void TimeDraw();

protected:
    void paintEvent(QPaintEvent *e);
    void closeEvent(QCloseEvent *e);

private:
    const int t_draw_interval;
    int bestscore;

    void Init();
    void Music_On();
    void Music_Off();
    QString MakeComment(bool is_win, int score, int bestscore, int win_condition);  //机器评价

    QFile *My_File;
    QTextStream *in;
    QTimer t_Draw;
    MusicPlay *music_bgm;
    MusicPlay *music_shoot;
    MyButton *MusicButton;
    Game_Logic *Game;

};
#endif // REVIEW_APP_H
