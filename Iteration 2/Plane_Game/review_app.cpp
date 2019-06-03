#include "review_app.h"

review_app::review_app(QString filename, int tdraw, int Score_BigEnemy, int Score_MidEnemy, int Score_SmallEnemy, QWidget *parent, Qt::WindowFlags flags)
    : QWidget(parent, flags), t_draw_interval(tdraw)
{
    Game = new Game_Logic(Score_BigEnemy, Score_MidEnemy, Score_SmallEnemy, 5);
    Game->Init();
    Game->GameStart();
    music_bgm = new MusicPlay(":/sound/sound/game_music.wav");  //设置背景音乐
    music_shoot = new MusicPlay(":/sound/sound/bullet.wav");  //设置子弹射击音乐
    MusicButton = new MyButton("", this);  //音乐按钮
    MusicButton->setGeometry(330, 730, 120, 40);
    MusicButton->setVisible(true);
    Music_On();

    //打开记录文件
    My_File = new QFile(filename);
    My_File->open(QIODevice::ReadOnly | QIODevice::Text);
    in = new QTextStream(&*My_File);

    //读取基本信息
    int BoomCount, Score, WinCondition, DSTime, GameDiffi;
    (*in) >> BoomCount >> Score >> WinCondition >> DSTime >> GameDiffi >> bestscore;
    Game->setBoomCount(BoomCount);
    Game->setScore(Score);
    Game->setWinCondition(WinCondition);
    Game->setDoubleShootTime(DSTime);
    Game->setGameDiffi(GameDiffi);

    t_Draw.setInterval(t_draw_interval);
    connect(&t_Draw, SIGNAL(timeout()), this, SLOT(TimeDraw()));
    t_Draw.start();
}

review_app::~review_app()
{
    delete My_File;
    delete in;
    delete music_bgm;
    delete music_shoot;
    delete MusicButton;
    delete Game;
}

void review_app::TimeDraw()  //绘图计时器 槽函数
{
    t_Draw.stop();  //暂停计时

    if(Game->is_GameOver())
    {
        t_Draw.start();
        return ;
    }

    int UseBoom, flag, x, y, speedx, speedy, hp, stntimes;
    QString kind;

    //读取是否使用炸弹
    (*in) >> UseBoom;

    //读取音乐状态
    (*in) >> flag;
    if(Game->is_Music_On() != flag)
    {
        if(flag)
            Music_On();
        else
            Music_Off();
        Game->setMusicOn(flag);
    }

    //读取玩家飞机状态
    (*in) >> x >> y;
    Game->setPlaneX(x);
    Game->setPlaney(y);

    //读取子弹状态
    Game->BulletListClear();
    while((*in) >> flag, flag != -1)
    {
        (*in) >> x >> y;
        Game->BulletListPush( Factory::GenBullet(flag == 1?"red":"blue", x, y) );
    }

    //读取敌机状态
    Game->EnemyListClear();
    while((*in) >> flag, flag != -1)
    {
        (*in) >> x >> y >> speedx >> speedy >> hp;
        switch(flag)
        {
            case 1: kind = "small"; break;
            case 2: kind = "mid"; break;
            case 3: kind = "big"; break;
        }
        Game->EnemyListPush( Factory::GenEnemy(kind, x, y, speedx, speedy, hp) );
    }

    //读取道具状态
    Game->RewardListClear();
    while((*in) >> flag, flag != -1)
    {
        (*in) >> x >> y >> speedx >> speedy >> stntimes;
        Game->RewardListPush( Factory::GenReward(flag == 1?"bullet_add":"boom_add", x , y, speedx, speedy, stntimes) );
    }

    if(UseBoom)  //若使用炸弹，直接调用函数清空敌机
        Game->Review_UseBoom();

    Game->is_Enemy_Down();  //敌方飞机消灭检测
    Game->is_Reward_Get();  //道具奖励获取检测
    if(Game->is_Plane_Down() == true)  //玩家飞机碰撞检测
    {
        t_Draw.stop();
        update();
        return;
    }

    update();
    t_Draw.start();
}

QString review_app::MakeComment(bool is_win, int score, int bestscore, int win_condition)
{
    if(is_win)  //若胜利
    {
        if(score >= bestscore)
            return "恭喜！赢了，还创造了新纪录！";
        else
            return "恭喜！胜利啦！";
    }
    else  //若失败
    {
        if(win_condition > score)  //在胜利模式下失败
        {
            if(score > bestscore)
                return "破纪录不代表胜利哟~";
            else if(win_condition - score < 10000)
                return "唉，就差那么一点点就赢了。";
            else
                return "革命尚未成功，同志仍需努力！";
        }
        else
        {
            if(score > bestscore)  //在无限模式下失败
                return "突破新纪录啦！";
            else
                return "表现不错！再接再厉！";
        }
    }

}

void review_app::paintEvent(QPaintEvent *e)  //绘图函数
{
    Q_UNUSED(e);
    QPainter painter(this);
    QFont font("微软雅黑",14);
    painter.setFont(font);
    painter.setPen(Qt::black);

    //绘制背景图
    painter.drawImage(QRect(0, 0, this->width(),this->height()), QImage(":/images/images/background.png"));

    //绘制玩家飞机
    painter.drawImage(QPoint(Game->getPlaneX() - Game->getPlaneWidth() / 2, Game->getPlaneY() - Game->getPlaneHeight() / 2), Game->getPlaneImage());

    //绘制子弹
    for(int i = 0; i < Game->get_BulletList_Size() ; i++)
        painter.drawImage(QPoint(Game->getBulletListX(i) - Game->getBulletListWidth(i) / 2, Game->getBulletListY(i) -Game->getBulletListHeight(i) / 2), Game->getBulletListImage(i));

    //绘制道具
    for(int i = 0; i < Game->get_RewardList_Size(); i++)
        painter.drawImage(QPoint(Game->getRewardListX(i) - Game->getRewardListWidth(i) / 2, Game->getRewardListY(i) -Game->getRewardListHeight(i) / 2),Game->getRewardListImage(i));

    //绘制敌方飞机
    for(int i = 0; i < Game->get_EnemyList_Size(); i++)
        painter.drawImage(QPoint(Game->getEnemyListX(i) - Game->getEnemyListWidth(i) / 2, Game->getEnemyListY(i) -Game->getEnemyListHeight(i) / 2),Game->getEnemyListImage(i));

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
    painter.drawText(QPointF(10, 30),"当前分数：" + QString::number(Game->getScore()));
    painter.drawText(QPointF(10, 80),"目标分数：" + ( Game->getWinCondition()>=0 ? QString::number(Game->getWinCondition()) : "∞" ));
    switch(Game->getGameDiffi())
    {
        case 0: painter.drawText(QPointF(10, 130),"游戏难度：简单"); break;
        case 1: painter.drawText(QPointF(10, 130),"游戏难度：困难"); break;
        case 2: painter.drawText(QPointF(10, 130),"游戏难度：噩梦"); break;
    }

    if(Game->is_GameOver())  //若一局游戏结束
    {
        QFont font("微软雅黑",12);
        painter.setFont(font);
        painter.setPen(Qt::black);
        painter.drawImage(QPoint(40, 210), QImage(":/images/images/text.png"));
        painter.drawImage(QPoint(90, 440), QImage(":/images/images/again.png"));
        painter.drawImage(QPoint(90, 510), QImage(":/images/images/gameover.png"));
        painter.drawImage(QPoint(90, 580), QImage(":/images/images/save.png"));
        painter.drawImage(QPoint(90, 650), QImage(":/images/images/back.png"));
        painter.drawText(QPointF(100, 250), MakeComment(Game->is_Win(), Game->getScore(), Game->getBestScore(), Game->getWinCondition()));
        painter.drawText(QPointF(100, 300), "你的得分：" + QString::number(Game->getScore()));
        painter.drawText(QPointF(100, 350), "目标得分：" + ( Game->getWinCondition()>=0 ? QString::number(Game->getWinCondition()) : " ∞" ));
        painter.drawText(QPointF(100, 400), "历史最高：" + QString::number(Game->getBestScore()));
        music_bgm->Stop();
        music_shoot->Stop();

        return ;
    }
}

void review_app::closeEvent(QCloseEvent *e)  //关闭事件
{
    Q_UNUSED(e);
    music_bgm->Stop();
    music_shoot->Stop();
}

void review_app::Music_On()  //开启音乐
{
    music_bgm->Play();
    music_shoot->Play();
    MusicButton->setText("音乐：开");
}

void review_app::Music_Off()  //关闭音乐
{
    music_bgm->Stop();
    music_shoot->Stop();
    MusicButton->setText("音乐：关");
}
