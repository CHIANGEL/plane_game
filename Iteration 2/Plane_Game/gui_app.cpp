#include "gui_app.h"
#include <QMessageBox>
#include <QInputDialog>
#include <QDir>
#include <QDebug>

gui_app::gui_app(int tdraw, int Score_BigEnemy, int Score_MidEnemy, int Score_SmallEnemy, int Plane_Key_Speed,
                 QWidget *parent, Qt::WindowFlags flags)
    : QWidget(parent, flags), t_draw_interval(tdraw)
{
    Game = new Game_Logic(Score_BigEnemy, Score_MidEnemy, Score_SmallEnemy, Plane_Key_Speed);
    music_bgm = new MusicPlay(":/sound/sound/game_music.wav");  //设置背景音乐
    music_shoot = new MusicPlay(":/sound/sound/bullet.wav");  //设置子弹射击音乐
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));

    StartInterface();
}

gui_app::~gui_app()
{
    delete Game;
    delete music_bgm;
    delete music_shoot;
    delete StartButton;
    delete LoadButton;
    delete ReviewButton;
}

void gui_app::StartInterface()  //游戏开始界面
{
    StartButton = new MyButton("开 始 游 戏", this);  //按钮初始化
    LoadButton = new MyButton("读 取 存 档", this);
    ReviewButton = new MyButton("精 彩 回 放", this);
    StartButton->setGeometry(90, 530, 300, 40);  //设置三个按钮的位置
    LoadButton->setGeometry(90, 590, 300, 40);
    ReviewButton->setGeometry(90, 650, 300, 40);

    connect(StartButton,SIGNAL(clicked()),this,SLOT(StartButtonPress()));  //连接按钮事件与槽函数
    connect(LoadButton,SIGNAL(clicked()),this,SLOT(LoadButtonPress()));
    connect(ReviewButton,SIGNAL(clicked()),this,SLOT(ReviewButtonPress()));

    InfiniteModel = new QRadioButton("无限模式", this);  //单选按钮初始化
    VictoryModel = new QRadioButton("胜利模式", this);
    EasyDiffi = new QRadioButton("简单模式", this);
    HardDiffi = new QRadioButton("困难模式", this);
    MareDiffi = new QRadioButton("噩梦模式", this);
    GroupModel = new QButtonGroup(this);
    GroupDiffi = new QButtonGroup(this);

    GroupModel->addButton(InfiniteModel);  //将单选按钮分组
    GroupModel->addButton(VictoryModel);
    GroupDiffi->addButton(EasyDiffi);
    GroupDiffi->addButton(HardDiffi);
    GroupDiffi->addButton(MareDiffi);
    InfiniteModel->setChecked(true);
    EasyDiffi->setChecked(true);

    InfiniteModel->setGeometry(80, 320, 100, 30);  //设置单选按钮位置
    VictoryModel->setGeometry(210, 320, 100, 30);
    EasyDiffi->setGeometry(80, 400, 100, 30);
    HardDiffi->setGeometry(210, 400, 100, 30);
    MareDiffi->setGeometry(340, 400, 100, 30);

}

void gui_app::Init()  //初始化
{
    Game->Init();
    Game->GameStart();
    music_bgm->Play();
    music_shoot->Play();

    QFile My_File("./NewRecord.txt");  //清空记录游戏过程的NewRecord文件，并记录基本信息
    My_File.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&My_File);
    out << Game->getInitBoomCount() << ' '<< Game->getScore() <<' '<< Game->getWinCondition() <<' '<<
           Game->getDSTime() <<' '<< Game->getGameDiffi() <<' '<< Game->getBestScore()<< endl;
    My_File.close();

    MusicButton = new MyButton("音乐：开", this);  //音乐按钮
    MusicButton->setGeometry(330, 730, 120, 40);
    MusicButton->setVisible(true);
    connect(MusicButton,SIGNAL(clicked()),this,SLOT(MusicButtonPress()));

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

QString gui_app::MakeComment(bool is_win, int score, int bestscore, int win_condition)
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

void gui_app::TimeDraw()  //计时器槽函数，检测、记录并绘制
{
    t_Draw.stop();  //暂停计时

    if(Game->is_Pause() || Game->is_GameOver())
    {
        t_Draw.start();
        return ;
    }

    Game->RecordStatus("./NewRecord.txt", 0);

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
    if(Game->is_Pause() || Game->is_GameOver())
        return ;
    Game->Shoot_Bullet(t_draw_interval);
}

void gui_app::TimeEnemy()  //计时器槽函数，产生敌军
{
    if(Game->is_Pause() || Game->is_GameOver())
        return ;
    Game->Enemy_Gene(this->width());
}

void gui_app::TimeReward()  //计时器槽函数，产生道具
{
    if(Game->is_Pause() || Game->is_GameOver())
        return ;
    Game->Reward_Gene(this->width());
}

void gui_app::StartButtonPress()  //“开始游戏”按钮 槽函数
{
    if(EasyDiffi->isChecked())  //若选中简单模式
    {
        Game->setWinCondition((rand() % 10 + 10) * 10000);  //设置胜利条件
        t_bullet_interval = 150;  //设置子弹射速
        t_enemy_interval = 500;  //设置敌机出现频率
        t_reward_interval = 5000;  //设置道具掉落频率
        Game->setInitBoomCount(5);  //设置初始炸弹数量
        Game->setGameDiffi(0);  //更改逻辑中的游戏难度
    }
    else if(HardDiffi->isChecked())  //若选中困难模式
    {
        Game->setWinCondition((rand() % 10 + 10) * 100000);  //设置胜利条件
        t_bullet_interval = 125;  //设置子弹射速
        t_enemy_interval = 350;  //设置敌机出现频率
        t_reward_interval = 6500;  //设置道具掉落频率
        Game->setInitBoomCount(3);  //设置初始炸弹数量
        Game->setGameDiffi(1);  //更改逻辑中的游戏难度
    }
    else if(MareDiffi->isChecked())  //若选中噩梦模式
    {
        Game->setWinCondition((rand() % 10 + 10) * 1000000);  //设置胜利条件
        t_bullet_interval = 100;  //设置子弹射速
        t_enemy_interval = 270;  //设置敌机出现频率
        t_reward_interval = 8000;  //设置道具掉落频率
        Game->setInitBoomCount(1);  //设置初始炸弹数量
        Game->setGameDiffi(2);  //更改逻辑中的游戏难度
    }

    if(InfiniteModel->isChecked())//若玩家选中无限模式
        Game->setWinCondition(-1);

    delete StartButton;  //清空游戏开始界面
    delete LoadButton;
    delete ReviewButton;
    delete InfiniteModel;
    delete VictoryModel;
    delete EasyDiffi;
    delete HardDiffi;
    delete MareDiffi;
    delete GroupModel;
    delete GroupDiffi;

    Init();  //开始游戏初始化
}

void gui_app::LoadButtonPress()  //“读取存档”按钮 槽函数
{
    QString fileName = QFileDialog::getOpenFileName(this);
    if(fileName.isEmpty())
        return ;

    Game->LoadArchive(fileName);

    delete StartButton;  //清空游戏开始界面
    delete LoadButton;
    delete ReviewButton;
    delete InfiniteModel;
    delete VictoryModel;
    delete EasyDiffi;
    delete HardDiffi;
    delete MareDiffi;
    delete GroupModel;
    delete GroupDiffi;

    switch(Game->getGameDiffi())  //根据读取的游戏难度调整计时器和初始炸弹
    {
        case 0: t_bullet_interval = 150;
                t_enemy_interval = 500;
                t_reward_interval = 5000;
                Game->setInitBoomCount(5);
                break;
        case 1: t_bullet_interval = 125;
                t_enemy_interval = 300;
                t_reward_interval = 6500;
                Game->setInitBoomCount(3);
                break;
        case 2: t_bullet_interval = 100;
                t_enemy_interval = 200;
                t_reward_interval = 8000;
                Game->setInitBoomCount(1);
                break;
    }

    QMessageBox box;
    box.setWindowTitle("读取存档");
    box.setText("读档成功，即将开始游戏！");
    box.addButton("好(&Y)",QMessageBox::YesRole);
    box.exec();

    MusicButton = new MyButton(Game->is_Music_On() == 1?"音乐：开":"音乐：关", this);  //音乐按钮
    MusicButton->setGeometry(330, 730, 120, 40);
    MusicButton->setVisible(true);
    connect(MusicButton,SIGNAL(clicked()),this,SLOT(MusicButtonPress()));
    if(Game->is_Music_On())
    {
        music_bgm->Play();
        music_shoot->Play();
    }

    QFile My_File("./NewRecord.txt");  //清空记录游戏过程的NewRecord文件，并记录基本信息
    My_File.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&My_File);
    out << Game->getBoomCount() << ' '<< Game->getScore() <<' '<< Game->getWinCondition() <<' '<< Game->getDSTime() <<' '<< Game->getGameDiffi() << endl;
    My_File.close();

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

void gui_app::ReviewButtonPress()  //“精彩回放”按钮 槽函数
{
    QString fileName = QFileDialog::getOpenFileName(this);
    if(fileName.isEmpty())
        return ;

    review_app *Review;  //构造一个“精彩回放”类的对象
    Review = new review_app(fileName, t_draw_interval, Game->getScoreBig(), Game->getScoreMid(), Game->getScoreSmall());
    Review->setFixedSize(480,800);
    Review->show();
}

void gui_app::MusicButtonPress()  //“音乐”按钮 槽函数
{
    if(!Game->is_Pause())  //若没有暂停，则不能开关音乐
        return;

    if(Game->is_Music_On() == true)
    {
        MusicButton->setText("音乐：关");
        music_bgm->Stop();
        music_shoot->Stop();
        Game->setMusicOn(false);
    }
    else
    {
        MusicButton->setText("音乐：开");
        music_bgm->Play();
        Game->setMusicOn(true);
    }
}

void gui_app::paintEvent(QPaintEvent *e)  //绘图函数
{
    Q_UNUSED(e);
    QPainter painter(this);
    QFont font("微软雅黑",14);
    painter.setFont(font);
    painter.setPen(Qt::black);

    //绘制背景图
    painter.drawImage(QRect(0, 0, this->width(),this->height()), QImage(":/images/images/background.png"));

    if(!Game->is_Start())  //若游戏未开始，则绘制开始界面，然后返回
    {
        painter.drawImage(QPoint(20, 90), QImage(":/images/images/title.png"));
        painter.drawText(QPoint(40, 300), "游戏模式：");
        painter.drawText(QPoint(40, 380), "游戏难度：");
        return ;
    }

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

    //绘制得分、目标分数和游戏模式
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

    if(Game->is_Pause())  //若处于暂停状态
    {
        QFont font("微软雅黑",22);
        painter.setFont(font);
        painter.setPen(Qt::black);
        painter.drawText(QPointF(this->width()/2-45, this->height()/2-50), QString("暂 停"));
        painter.drawImage((QPointF(90, 390)), QImage(":/images/images/record.png"));
        painter.drawImage((QPointF(90, 480)), QImage(":/images/images/back.png"));
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
        QFile My_File("./NewRecord.txt");  //清空记录游戏过程的NewRecord文件
        My_File.open(QIODevice::WriteOnly | QIODevice::Text);
        My_File.close();
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
    else if(Flag == 3)  //表示保存该局游戏
    {
        QString fileName = QFileDialog::getSaveFileName(this, tr("保存游戏记录"), "未命名.txt");
        if(!fileName.isEmpty())
        {
            Game->SaveRecord("./NewRecord.txt",fileName);
            QMessageBox box;
            box.setWindowTitle("保存游戏记录");
            box.setText("游戏记录保存成功！");
            box.addButton("好(&Y)",QMessageBox::YesRole);
            box.exec();
        }
    }
    else if(Flag == 4)  //表示存档该局游戏
    {
        QString fileName = QFileDialog::getSaveFileName(this, tr("存档游戏"), "未命名.txt");
        if(!fileName.isEmpty())
        {
            Game->SaveArchive(fileName);
            QMessageBox box;
            box.setWindowTitle("存档游戏");
            box.setText("成功存档！        ");
            box.addButton("好(&Y)",QMessageBox::YesRole);
            box.exec();
        }
    }
    else if(Flag == 5)  //表示返回菜单
    {
        Widget = new gui_app(t_draw_interval, Game->getScoreBig(), Game->getScoreMid(), Game->getScoreSmall(), Game->getKeySpeed());
        Widget->setFixedSize(480,800);
        Widget->show();
        this->close();
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
        if(Game->is_Music_On())
            music_shoot->Play();  //播放射击音乐
        this->setCursor(Qt::BlankCursor);  //隐藏光标
        this->cursor().setPos(QPoint(Game->getPlaneX() + this->x(), Game->getPlaneY() + this->y()));  //将光标移回飞机位置
    }
    else if(Flag == -1)  //返回值为-1，不做窗口改变
    {
        return ;
    }
}

void gui_app::closeEvent(QCloseEvent *e)  //窗口关闭事件
{
    Q_UNUSED(e);
    QFile My_File("./NewRecord.txt");  //清空记录游戏过程的NewRecord文件
    My_File.open(QIODevice::WriteOnly | QIODevice::Text);
    My_File.close();
    music_bgm->Stop();
    music_shoot->Stop();
}
