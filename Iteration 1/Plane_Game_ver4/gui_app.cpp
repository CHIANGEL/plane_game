#include "gui_app.h"
#include <QDebug>

gui_app::gui_app(int tdraw, int tbullet, int tenemy, int treward,
                 int Score_BigEnemy, int Score_MidEnemy, int Score_SmallEnemy,
                 int Init_BoomCount, int Plane_Key_Speed, int win_condition,
                 QWidget *parent, Qt::WindowFlags flags)
    : QWidget(parent, flags), t_draw_interval(tdraw), t_bullet_interval(tbullet), t_enemy_interval(tenemy), t_reward_interval(treward)
{
    Game = new Game_Logic(Score_BigEnemy, Score_MidEnemy, Score_SmallEnemy, Init_BoomCount, Plane_Key_Speed, win_condition);
    music_bgm = new MusicPlay(":/sound/sound/game_music.wav");  //设置背景音乐
    music_shoot = new MusicPlay(":/sound/sound/bullet.wav");  //设置子弹射击音乐
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));

    Init();
}

gui_app::~gui_app()
{
    delete Game;
    delete music_bgm;
    delete music_shoot;
}

void gui_app::Init()  //初始化
{
    Game->Init();
    music_bgm->Play();
    music_shoot->Play();

    setMouseTracking(true);  //设置鼠标追踪
    this->setCursor(Qt::BlankCursor);  //将光标设为不可见
    this->cursor().setPos(QPoint(Game->getPlaneX() + this->x(), Game->getPlaneY() + this->y()));

    t_Draw.setInterval(t_draw_interval);  //设置计时器时间
    t_Bullet.setInterval(t_bullet_interval);
    t_Enemy.setInterval(t_enemy_interval);
    t_Reward.setInterval(t_reward_interval);

    connect(&t_Draw,SIGNAL(timeout()),this,SLOT(TimeDraw()));  //设置计时器的信号与槽
    connect(&t_Bullet,SIGNAL(timeout()),this,SLOT(TimeBullet()));
    connect(&t_Enemy,SIGNAL(timeout()),this,SLOT(TimeEnemy()));
    connect(&t_Reward,SIGNAL(timeout()),this,SLOT(TimeReward()));

    t_Draw.start();  //开始计时
    t_Bullet.start();
    t_Enemy.start();
    t_Reward.start();
}

void gui_app::TimeDraw()  //计时器槽函数，检测并绘制
{
    t_Draw.stop();  //暂停计时

    if(Game->is_Pause() || Game->is_GameOver())
    {
        t_Draw.start();
        return ;
    }

    Game->is_Enemy_Down();  //敌方飞机消灭检测
    Game->is_Reward_Get();  //道具奖励获取检测
    if(Game->is_Plane_Down() == true)  //玩家飞机碰撞检测
    {
        t_Draw.stop();
        this->setCursor(Qt::ArrowCursor);  //一局游戏结束，重现光标
        update();
        return;
    }

    update();
    t_Draw.start();
}

void gui_app::TimeBullet()  //计时器槽函数，发射子弹
{
    Game->Shoot_Bullet(t_draw_interval);
}

void gui_app::TimeEnemy()  //计时器槽函数，产生敌军
{
    Game->Enemy_Gene(this->width());
}

void gui_app::TimeReward()  //计时器槽函数，产生道具
{
    Game->Reward_Gene(this->width());
}

void gui_app::paintEvent(QPaintEvent *e)  //绘图函数
{
    QPainter painter(this);

    //绘制背景图
    painter.drawImage(QRect(0, 0, this->width(),this->height()), QImage(":/images/images/background.png"));

    //绘制玩家飞机
    painter.drawImage(QPoint(Game->getPlaneX() - Game->getPlaneWidth() / 2, Game->getPlaneY() - Game->getPlaneHeight() / 2), Game->getPlaneImage());

    //绘制子弹
    for(int i = 0; i < Game->get_BulletList_Size() ; i++)
        painter.drawImage(QPoint(Game->getBulletListX(i) - Game->getBulletListWidth(i) / 2, Game->getBulletListY(i) -Game->getBulletListHeight(i) / 2), Game->getBulletListImage(i));
    Game->Move_Bullet();

    //绘制道具
    for(int i = 0; i < Game->get_RewardList_Size(); i++)
        painter.drawImage(QPoint(Game->getRewardListX(i) - Game->getRewardListWidth(i) / 2, Game->getRewardListY(i) -Game->getRewardListHeight(i) / 2),Game->getRewardListImage(i));
    Game->Move_Reward(this->height());


    //绘制敌方飞机
    for(int i = 0; i < Game->get_EnemyList_Size(); i++)
        painter.drawImage(QPoint(Game->getEnemyListX(i) - Game->getEnemyListWidth(i) / 2, Game->getEnemyListY(i) -Game->getEnemyListHeight(i) / 2),Game->getEnemyListImage(i));
    Game->Move_Enemy(this->width(), this->height());

    //在左下角绘制玩家已有炸弹数量
    if(Game->getBoomCount() > 0)
    {
        QImage boom_icon = Factory::GetBoomIcon();
        painter.drawImage(QPoint(10, this->height() - 40 - boom_icon.height()), boom_icon);  //绘制炸弹图标
        QFont font("微软雅黑",18);  //设置字体、大小和颜色
        painter.setFont(font);
        painter.setPen(Qt::black);
        painter.drawText(QPoint(10 + boom_icon.width() + 5,this->height() -10 - boom_icon.height()),"×" + QString::number(Game->getBoomCount()));  //写字
    }

    //绘制爆炸效果
    for (int i = 0; i < Game->get_ExplosionList_Size() ; i++)
    {
        QImage temp = Game->getExplosionListImage(i, Game->getExplosionListCounter(i) / (Game->getExplosionListStnTimes(i) /Game->get_ExplosionList_ImagesSize(i)));
        painter.drawImage(QPoint(Game->getExplosionListX(i) - temp.width() / 2, Game->getExplosionListY(i) -temp.height() / 2), temp);
        Game->setExplosionListCounter(i, Game->getExplosionListCounter(i) + 24);
        if (Game->getExplosionListCounter(i) > Game->getExplosionListStnTimes(i))
            Game->removeExplosionList(i);
    }

    //绘制得分
    QFont font("微软雅黑",14);
    painter.setFont(font);
    painter.setPen(Qt::black);
    painter.drawText(QPointF(10, 30),"分数:" + QString::number(Game->getScore()));
    painter.drawText(QPointF(10, 80),"目标分数 :" + ( Game->getWinCondition()>=0 ? QString::number(Game->getWinCondition()) : " ∞" ));

    if(Game->is_GameOver())  //若一局游戏结束
    {
        painter.drawImage(QPoint(90, 450), QImage(":/images/images/again.png"));
        painter.drawImage(QPoint(90, 530), QImage(":/images/images/gameover.png"));
        music_bgm->Stop();
        music_shoot->Stop();

        if(Game->is_Win())  //若玩家获胜
        {
            painter.drawText(QPointF(170, 300),"恭喜！你赢了！");
            painter.drawText(QPointF(120, 350),"你的得分：" + QString::number(Game->getScore()));
            painter.drawText(QPointF(120, 400),"目标得分：" + ( Game->getWinCondition()>=0 ? QString::number(Game->getWinCondition()) : " ∞" ));
        }
        else
        {
            painter.drawText(QPointF(170, 300),"真遗憾，你输了");
            painter.drawText(QPointF(120, 350),"你的得分：" + QString::number(Game->getScore()));
            painter.drawText(QPointF(120, 400),"目标得分：" + ( Game->getWinCondition()>=0 ? QString::number(Game->getWinCondition()) : " ∞" ));
        }

        return ;
    }
    if(Game->is_Pause())  //若处于暂停状态
    {
        QFont font("微软雅黑",22);
        painter.setFont(font);
        painter.setPen(Qt::black);
        painter.drawText(QPointF(this->width()/2-30, this->height()/2-50),QString("暂 停"));
        return;
    }
}

void gui_app::mouseMoveEvent(QMouseEvent *e)  //鼠标移动事件
{
    int Flag = Game->Mouse_Move(e->x(), e->y());  //产生事件，调用逻辑

    if(Flag == 1)  //返回值为1，变光标为手型
    {
        this->setCursor(Qt::PointingHandCursor);
    }
    else if(Flag == 2)  //返回值为2，变光标为箭型
    {
        this->setCursor(Qt::ArrowCursor);
    }
    else if(Flag == -1)  //返回值为-1，不做窗口改变
    {
        return ;
    }
}

void gui_app::mousePressEvent(QMouseEvent *e)  //鼠标点击事件
{
    int Flag = Game->Mouse_Press(e->x(), e->y(), e->button());  //产生事件，调用逻辑

    if(Flag == 0)  //返回值为0，结束游戏
    {
        this->close();
    }
    else if(Flag == 1)  //返回值为1，重新开始一局游戏
    {
        Init();
        update();
        t_Draw.start();
    }
    else if(Flag == 2)  //表示使用炸弹后获得胜利
    {
        update();
    }
    else if(Flag == -1)  //返回值为-1，不做窗口改变
    {
        return ;
    }
}

void gui_app::keyPressEvent(QKeyEvent *e)  //键盘事件
{
    int Flag = Game->Key_Press(Qt::Key(e->key()));  //产生事件，调用逻辑

    if(Flag == 1)  //返回值为1，暂停游戏
    {
        t_Draw.stop(); //停止计时
        music_shoot->Stop();  //射击音乐停止，背景音乐不停止
        this->setCursor(Qt::ArrowCursor);  //重现光标
        update();
    }
    else if(Flag == 2)  //返回值为2，继续游戏
    {
        t_Draw.start();  //开始计时
        music_shoot->Play();  //播放射击音乐
        this->setCursor(Qt::BlankCursor);  //隐藏光标
        this->cursor().setPos(QPoint(Game->getPlaneX() + this->x(), Game->getPlaneY() + this->y()));  //将光标移回飞机位置
    }
    else if(Flag == -1)  //返回值为-1，不做窗口改变
    {
        return ;
    }
}

