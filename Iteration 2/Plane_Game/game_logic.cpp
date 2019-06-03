#include "game_logic.h"
#include <QTime>
#include <QFile>
#include <QDir>
#include <QDate>
#include <QDebug>

Game_Logic::Game_Logic(int big, int mid, int small, int Key_Speed)
    :Score_BigEnemy(big), Score_MidEnemy(mid), Score_SmallEnemy(small),
     Plane_Key_Speed(Key_Speed)
{
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    Init();
}

void Game_Logic::Init()  //初始化
{
    score = 0;
    pause = false;
    b_GameOver = false;
    b_GameStart = false;
    b_Win = false;
    b_Music_On = true;
    double_bullet_stnTime = 0;
    boom_count = Init_BoomCount;

    QFile My_File("./BestScore.txt");
    My_File.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream in(&My_File);
    in >> BestScore;

    plane = Factory::GenPlane();  //产生我方飞机
    enemy_list.clear();  //清空敌机列表
    bullet_list.clear();  //清空子弹列表
    explosion_list.clear();  //清空爆炸效果
    reward_list.clear();  //清空奖励道具
}

int Game_Logic::Mouse_Move(int x, int y)  //鼠标移动事件
{
    if(b_GameOver)  //若是一局游戏结束的状态
    {
        if( x-90 <=300 && x-90>=0 &&
            y-440<=40  && y-440>=0 )
        {
            return 1;  //位于特殊位置，返回1改变光标形状this->setCursor(Qt::PointingHandCursor);
        }
        else if( x-90 <=300 && x-90>=0 &&
                 y-510<=40  && y-510>=0 )
        {
            return 1;  //位于特殊位置，返回1改变光标形状
        }
        else if( x-90 <=300 && x-90>=0 &&
                 y-580<=40  && y-580>=0 )
        {
            return 1;  //位于特殊位置，返回1改变光标形状
        }
        else if( x-90 <=300 && x-90>=0 &&
                 y-650<=40  && y-650>=0 )
        {
            return 1;  //位于特殊位置，返回1改变光标形状
        }
        else
        {
            return 2;  //无位于特殊位置，返回2改变光标形状
        }
    }
    else if(!pause)
    {
        //若既非GameOver，也非暂停，则让玩家飞机跟踪光标移动
        //返回3表示光标形状不做变化
        plane.setX(x);
        plane.setY(y);
        return 3;
    }
    else if(pause)
    {
        if(x-90 <=300 && x-90>=0 &&
           y-390<=40  && y-390>=0 )
        {
            return 1;  //暂停状态下，鼠标停留在“存档”按钮，改变光标为手型
        }
        else if(x-90 <=300 && x-90>=0 &&
                y-480<=40  && y-480>=0 )
        {
            return 1;  //暂停状态下，鼠标停留在“存档”按钮，改变光标为手型
        }
        else
        {
            return 2; //暂停状态下，没有停留在按钮上，光标变回箭型
        }
    }
    return -1;
}

int Game_Logic::Mouse_Press(int x, int y, Qt::MouseButton button)  //鼠标点击事件
{
    if(!b_GameOver && !pause && button == Qt::RightButton)  //游戏中，右键使用炸弹
    {
        if(boom_count > 0)
        {
            RecordStatus("./NewRecord.txt", 1);  //记录一次使用炸弹的记录
            boom_count--;
            for(int i = 0; i < enemy_list.size(); i++)  //遍历所有敌机
            {
                //玩家得分
                if (enemy_list[i].getName() == "small") score += Score_SmallEnemy;
                else if (enemy_list[i].getName() == "mid") score += Score_MidEnemy;
                else if (enemy_list[i].getName() == "big") score += Score_BigEnemy;

                //爆炸效果
                if(enemy_list[i].getName() == "small")
                    explosion_list.push_back(Factory::GenExplosion("small", enemy_list[i].getX(), enemy_list[i].getY()));
                else if(enemy_list[i].getName() == "mid")
                    explosion_list.push_back(Factory::GenExplosion("mid", enemy_list[i].getX(), enemy_list[i].getY()));
                else if (enemy_list[i].getName() == "big")
                    explosion_list.push_back(Factory::GenExplosion("big", enemy_list[i].getX(), enemy_list[i].getY()));
            }
            enemy_list.clear();  //直接清空敌机列表

            if(score >= Win_Condition && Win_Condition > 0)  //若在非无限模式下达到胜利条件
            {
                b_Win = true;
                b_GameOver = true;
                if(score > BestScore)  //若分数超过历史最高，则更新
                    UpdateBestScore();
                return 2;
            }
            else
            {
                return -1;
            }
        }
    }
    if(pause && button == Qt::LeftButton)  //若处于暂停状态，则判断左键选择
    {
        if( x-90 <=300 && x-90>=0 &&
            y-390<=40  && y-390>=0 )  //点击存档游戏
        {
            return 4;
        }
        else if(x-90 <=300 && x-90>=0 &&
                y-480<=40  && y-480>=0 )
        {
            return 5;  //点击返回菜单
        }
    }
    if(b_GameOver && button == Qt::LeftButton)  //若一局游戏结束，则判断左键选择
    {
        if( x-90 <=300 && x-90>=0 &&
            y-440<=40  && y-440>=0 )
        {
            return 1;  //点击了“重新开始”按钮，返回1重新开始
        }
        else if( x-90 <=300 && x-90>=0 &&
            y-510<=40  && y-510>=0 )
        {
            return 0;  //点击了“游戏结束”按钮，返回0关闭窗口
        }
        else if( x-90 <=300 && x-90>=0 &&
            y-580<=40  && y-580>=0 )
        {
            return 3;  //点击了“保存游戏”按钮，返回3
        }
        else if( x-90 <=300 && x-90>=0 &&
                 y-650<=40  && y-650>=0 )
        {
            return 5;  //点击“返回菜单”按钮，返回5
        }

    }
    return -1;
}

int Game_Logic::Key_Press(Qt::Key key)  //键盘事件
{
    if(b_GameOver)  //游戏结束的话忽略键盘事件
    {
        return -1;  //返回-1不做窗口改变
    }
    if(key == Qt::Key_P)
    {
        //游戏未结束，则按空格键暂停游戏/继续游戏
        pause = !pause;
        if(pause)
        {
            return 1;  //若暂停游戏，返回1
        }
        else
        {
            return 2;  //若继续游戏,返回2
        }
    }
    else if(key == Qt::Key_Left)
    {
        plane.setX(plane.getX() - Plane_Key_Speed);
        return -1;
    }
    else if(key == Qt::Key_Right)
    {
        plane.setX(plane.getX() + Plane_Key_Speed);
        return -1;
    }
    else if(key == Qt::Key_Up)
    {
        plane.setY(plane.getY() - Plane_Key_Speed);
        return -1;
    }
    else if(key == Qt::Key_Down)
    {
        plane.setY(plane.getY() + Plane_Key_Speed);
        return -1;
    }
    return -1;
}

void Game_Logic::Move_Bullet()  //绘制后移动子弹
{
    for(int i = 0; i < bullet_list.size(); i++)
    {
        bullet_list[i].setY(bullet_list[i].getY() - bullet_list[i].getSpeedY());  //绘制完后移动子弹
        if(bullet_list[i].getY() < -40)  //若出了边界则移除子弹
        {
            bullet_list.removeAt(i);
        }
    }
}

void Game_Logic::Move_Enemy(int Widgetwidth, int WidgetHeight)  //绘制后移动敌机
{
    for(int i = 0; i < enemy_list.size(); i++)
    {
        if(GameDiffi == 2) //若是噩梦模式，则调用机器智能
        {
            AI_Approach();
            AI_Avoid();
        }

        enemy_list[i].setY(enemy_list[i].getY() + enemy_list[i].getSpeedY());
        enemy_list[i].setX(enemy_list[i].getX() + enemy_list[i].getSpeedX());

        if(enemy_list[i].getX() > Widgetwidth || enemy_list[i].getX() < 0)  //若碰到左右边界，则水平速度取反
        {
            enemy_list[i].setSpeedX(- enemy_list[i].getSpeedX());
        }
        if(enemy_list[i].getY() > WidgetHeight + 20)  //若出了下界，则移除
        {
            enemy_list.removeAt(i);
        }
    }
}

void Game_Logic::Move_Reward(int WidgetHeight)  //绘制后移动道具
{
    for(int i = 0; i < reward_list.size(); i++)
    {
        reward_list[i].setX(reward_list[i].getX() + reward_list[i].getSpeedX());
        reward_list[i].setY(reward_list[i].getY() + reward_list[i].getSpeedY());
        if(reward_list[i].getY() > WidgetHeight + 20)
        {
            reward_list.removeAt(i);
        }
    }
}

void Game_Logic::AI_Approach()  //游戏智能：敌机向玩家飞机移动
{
    int NewSpeedX, SubX;
    for(int i = 0; i < enemy_list.size(); i++)
    {
        SubX = enemy_list[i].getX() - plane.getX();
        if(SubX > 50)  //转向条件：大型敌机与玩家飞机水平差值超过50
        {
            NewSpeedX = enemy_list[i].getSpeedX() == 0 ? -1:-qAbs(enemy_list[i].getSpeedX());
            enemy_list[i].setSpeedX( NewSpeedX );
        }
        else if(SubX < -50)
        {
            NewSpeedX = enemy_list[i].getSpeedX() == 0 ? 1:qAbs(enemy_list[i].getSpeedX());
            enemy_list[i].setSpeedX( NewSpeedX );
        }
    }
}

void Game_Logic::AI_Avoid()  //游戏智能：中、小型敌机躲闪玩家子弹
{
    int NewSpeedX, SubX, SubY;
    for(int i = 0; i < enemy_list.size(); i++)  //遍历所有敌机
    {
        if(enemy_list[i].getName() == "small" || enemy_list[i].getName() == "mid")  //若是中、小型敌机
        {
            for(int j = 0; j < bullet_list.size(); j++)  //遍历所有子弹
            {
                SubX = enemy_list[i].getX() - bullet_list[j].getX();
                SubY = bullet_list[j].getY() - enemy_list[i].getY();
                if (SubX > 0 && SubX < (bullet_list[j].getWidth() + enemy_list[i].getWidth())/2+5 &&
                    SubY > 0 && SubY < (bullet_list[j].getHeight() + enemy_list[i].getHeight())/2+5)
                {
                    NewSpeedX = enemy_list[i].getSpeedX() == 0 ? 1:qAbs(enemy_list[i].getSpeedX());
                    enemy_list[i].setSpeedX( NewSpeedX );
                }
                else if (SubX < 0 && -SubX < (bullet_list[j].getWidth() + enemy_list[i].getWidth())/2+5 &&
                         SubY > 0 && SubY < (bullet_list[j].getHeight() + enemy_list[i].getHeight())/2+5)
                {
                    NewSpeedX = enemy_list[i].getSpeedX() == 0 ? -1:-qAbs(enemy_list[i].getSpeedX());
                    enemy_list[i].setSpeedX( NewSpeedX );
                }
            }
        }
    }
}

void Game_Logic::RecordStatus(QString filename, bool UseBoom)  //记录一次游戏状态
{
    QFile My_File(filename);
    My_File.open(QIODevice::Append | QIODevice::Text);//以只写和文本模式打开文件
    QTextStream out(&My_File);

    //表明是否使用炸弹
    out << UseBoom <<endl;

    //记录音乐状态
    out << b_Music_On <<endl;

    //记录玩家飞机状态
    out << plane.getX() <<' '<< plane.getY() <<endl;

    //记录子弹状态
    for(int i = 0; i < bullet_list.size() ; i++)
    {
        if(bullet_list[i].getName() == "red")
            out << 1 << endl;  //1代表红子弹
        else
            out << 2 << endl;  //2代表蓝子弹
        out << bullet_list[i].getX() <<' '<< bullet_list[i].getY() <<' '<<endl;
    }
    out << -1 << endl; //以-1表示结束

    //记录敌机状态
    for(int i = 0; i < enemy_list.size(); i++)
    {
        if(enemy_list[i].getName() == "small")
          out << 1 << endl;
        else if(enemy_list[i].getName() == "mid")
          out << 2 << endl;
        else if(enemy_list[i].getName() == "big")
          out << 3 << endl;
        out << enemy_list[i].getX() <<' '<< enemy_list[i].getY() <<' '<<
               enemy_list[i].getSpeedX() <<' '<< enemy_list[i].getSpeedY() <<endl;
        out << enemy_list[i].getHP() <<endl;
    }
    out << -1 << endl; //以-1表示结束

    //记录道具状态
    for(int i = 0; i < reward_list.size() ; i++)
    {
        if(reward_list[i].getName() == "bullet_add")
            out << 1 << endl;  //1代表双弹
        else
            out << 2 << endl;  //2代表炸弹
        out << reward_list[i].getX() <<' '<< reward_list[i].getY() <<' '<<
               reward_list[i].getSpeedX() <<' '<< reward_list[i].getSpeedY() <<endl;
        out << reward_list[i].getStnTimes() <<endl;
    }
    out << -1 << endl; //以-1表示结束

    My_File.flush();    //刷新文件，将所有数据写入文件
    My_File.close();
}

void Game_Logic::SaveRecord(QString srcPath, QString desPath)  //保存一局游戏记录
{
    QFile srcFile(srcPath);
    QFile desFile(desPath);
    srcFile.open(QIODevice::ReadOnly | QIODevice::Text);
    desFile.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream in(&srcFile);
    QTextStream out(&desFile);

    QString temp = in.readAll();  //将暂存文件内容全部读出
    out << temp;  //将内容全部写入记录保存文件
    desFile.flush();    //刷新文件，将所有数据写入文件

    srcFile.close();
    desFile.close();
}

void Game_Logic::SaveArchive(QString filename)  //存档游戏
{
    QFile My_File(filename);
    My_File.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&My_File);

    //记录当前拥有的炸弹数量、当前得分、目标分数、双弹时间、游戏难度和三个实体的产生频率
    out << boom_count <<' '<< score <<' '<< Win_Condition <<' '<< double_bullet_stnTime <<' '<< GameDiffi << endl;

    //记录音乐状态
    out << b_Music_On <<endl;

    //记录玩家飞机状态
    out << plane.getX() <<' '<< plane.getY() <<' '<<
           plane.getWidth() <<' '<< plane.getHeight() <<' '<<endl;

    //记录子弹状态
    for(int i = 0; i < bullet_list.size() ; i++)
    {
        if(bullet_list[i].getName() == "red")
            out << 1 << endl;  //1代表红子弹
        else
            out << 2 << endl;  //2代表蓝子弹
        out << bullet_list[i].getX() <<' '<< bullet_list[i].getY() <<' '<<
              bullet_list[i].getWidth() <<' '<< bullet_list[i].getHeight() <<' '<<
              bullet_list[i].getSpeedX() <<' '<< bullet_list[i].getSpeedY() <<endl;
    }
    out << -1 << endl; //以-1表示结束

    //记录敌机状态
    for(int i = 0; i < enemy_list.size(); i++)
    {
        if(enemy_list[i].getName() == "small")
          out << 1 << endl;
        else if(enemy_list[i].getName() == "mid")
          out << 2 << endl;
        else if(enemy_list[i].getName() == "big")
          out << 3 << endl;
        out << enemy_list[i].getX() <<' '<< enemy_list[i].getY() <<' '<<
              enemy_list[i].getWidth() <<' '<< enemy_list[i].getHeight() <<' '<<
              enemy_list[i].getSpeedX() <<' '<< enemy_list[i].getSpeedY() <<endl;
        out << enemy_list[i].getHP() <<endl;
    }
    out << -1 << endl; //以-1表示结束

    //记录道具状态
    for(int i = 0; i < reward_list.size() ; i++)
    {
        if(reward_list[i].getName() == "bullet_add")
            out << 1 << endl;  //1代表双弹
        else
            out << 2 << endl;  //2代表炸弹
        out << reward_list[i].getX() <<' '<< reward_list[i].getY() <<' '<<
               reward_list[i].getWidth() <<' '<< reward_list[i].getHeight() <<' '<<
               reward_list[i].getSpeedX() <<' '<< reward_list[i].getSpeedY() <<endl;
        out << reward_list[i].getStnTimes() <<endl;
    }
    out << -1 << endl; //以-1表示结束

    My_File.close();
}

void Game_Logic::LoadArchive(QString filename)  //读档游戏
{
    QFile My_File(filename);
    My_File.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream in(&My_File);
    int flag, x, y, width, height, speedx, speedy, hp, stntimes;

    //先读取游戏基本信息
    in >> Init_BoomCount >> score >> Win_Condition >> double_bullet_stnTime >> GameDiffi;

    boom_count = Init_BoomCount;
    pause = false;
    b_GameOver = false;
    b_GameStart = true;
    b_Win = false;
    b_Music_On = true;
    explosion_list.clear();

    //读取音乐状态
    in >> flag;
    b_Music_On = flag == 1?true:false;

    //读取玩家飞机状态
    in >> x >> y >> width >> height;
    plane.setX(x);
    plane.setY(y);
    plane.setWidth(width);
    plane.setHeight(height);

    //读取子弹状态
    bullet_list.clear();
    while(in >> flag, flag != -1)
    {
        in >> x >> y >> width >> height >> speedx >> speedy;
        if(flag == 1)
        {
            Bullet temp("red", x, y, width, height, speedx, speedy, QImage(":/images/images/bullet1.png"));
            bullet_list.push_back(temp);
        }
        else
        {
            Bullet temp("blue", x, y, width, height, speedx, speedy, QImage(":/images/images/bullet2.png"));
            bullet_list.push_back(temp);
        }
    }

    //读取敌机状态
    enemy_list.clear();
    while(in >> flag, flag != -1)
    {
        in >> x >> y >> width >> height >> speedx >> speedy >> hp;
        if(flag == 1)
        {
            Enemy temp("small", x, y, width, height, speedx, speedy, hp, QImage(":/images/images/enemy1.png"));
            enemy_list.push_back(temp);
        }
        else if(flag == 2)
        {
            Enemy temp("mid", x, y, width, height, speedx, speedy, hp, QImage(":/images/images/enemy2.png"));
            enemy_list.push_back(temp);
        }
        else
        {
            Enemy temp("big", x, y, width, height, speedx, speedy, hp, QImage(":/images/images/enemy3_n1.png"));
            enemy_list.push_back(temp);
        }
    }

    //读取道具状态
    reward_list.clear();
    while(in >> flag, flag != -1)
    {
        in >> x >> y >> width >> height >> speedx >> speedy >> stntimes;
        if(flag == 1)
        {
            Reward temp("bullet_add", x, y, width, height, speedx, speedy, stntimes, QImage(":/images/images/bullet_supply.png"));
            reward_list.push_back(temp);
        }
        else
        {
            Reward temp("boom_add", x, y, width, height, speedx, speedy, stntimes, QImage(":/images/images/bomb_supply.png"));
            reward_list.push_back(temp);
        }
    }

    My_File.close();
}

void Game_Logic::Shoot_Bullet(int t_draw_interval)  //发射子弹
{
    if(double_bullet_stnTime > 0)  //双弹未结束
    {
        bullet_list.push_back(Factory::GenBullet("blue", plane.getX() - 7, plane.getY() - 50));
        bullet_list.push_back(Factory::GenBullet("blue", plane.getX() + 7, plane.getY() - 50));
        double_bullet_stnTime = double_bullet_stnTime - t_draw_interval;
    }
    else
    {
        bullet_list.push_back(Factory::GenBullet("red", plane.getX(), plane.getY() - 50));
        bullet_list.push_back(Factory::GenBullet("red", plane.getX(), plane.getY() - 50));
    }
}

void Game_Logic::Enemy_Gene(int WidgetWidth)  //产生新的敌机，数量比例为small:mid:big = 5:3:2 ，基础速度比例为2:1 = 1:4
{
    int speedBase = 0;
    if (qrand() % 10 < 8)
        speedBase = 1;
    else if (qrand() % 10 < 10)
        speedBase = 2;

    if (qrand() % 10 < 5)
    {
        enemy_list.push_back(Factory::GenEnemy("small", speedBase, WidgetWidth, GameDiffi));
    }
    else if(qrand() % 10 < 8)
    {
        enemy_list.push_back(Factory::GenEnemy("mid", speedBase, WidgetWidth, GameDiffi));
    }
    else if(qrand() % 10 < 10)
    {
        enemy_list.push_back(Factory::GenEnemy("big", speedBase, WidgetWidth, GameDiffi));
    }
}

void Game_Logic::Reward_Gene(int WidgetWidth)  //产生奖励道具
{
    if (qrand() % 2 == 0)
    {
        reward_list.push_back(Factory::GenReward("bullet_add", qrand()%(WidgetWidth)+50, 0));
    }
    else
    {
        reward_list.push_back(Factory::GenReward("boom_add", qrand()%(WidgetWidth)+50, 0));
    }
}

void Game_Logic::is_Enemy_Down()  //敌机消灭检测
{
    for(int i = 0; i < enemy_list.size(); i++)  //遍历所有敌机
    {
        for(int j = 0; j < bullet_list.size(); j++)  //遍历所有子弹
        {
            //若两者横纵坐标差值的绝对值 均小于 两者对应长/宽的平均值，则认为子弹射中敌机
            if (qAbs(bullet_list[j].getX() - enemy_list[i].getX()) < (bullet_list[j].getWidth() + enemy_list[i].getWidth()) / 2 && qAbs(bullet_list[j].getY() - enemy_list[i].getY()) < (bullet_list[j].getHeight() + enemy_list[i].getHeight()) / 2)
            {
                enemy_list[i].HP_down();
                if (enemy_list[i].getHP() == 0)  //敌机血量为0，被消灭
                {
                    //玩家获得分数
                    if(enemy_list[i].getName() == "small") score += Score_SmallEnemy;
                    else if (enemy_list[i].getName() == "mid") score += Score_MidEnemy;
                    else if (enemy_list[i].getName() == "big") score += Score_BigEnemy;



                    if(score >= Win_Condition && Win_Condition > 0)  //在非无限模式下达到胜利条件
                    {
                        b_Win = true;
                        b_GameOver = true;
                        if(score > BestScore)  //若分数超过历史最高，则更新
                            UpdateBestScore();
                    }

                    //产生爆炸效果
                    if(enemy_list[i].getName() == "small")
                        explosion_list.push_back(Factory::GenExplosion("small", enemy_list[i].getX(), enemy_list[i].getY()));
                    else if(enemy_list[i].getName() == "mid")
                        explosion_list.push_back(Factory::GenExplosion("mid", enemy_list[i].getX(), enemy_list[i].getY()));
                    else if (enemy_list[i].getName() == "big")
                        explosion_list.push_back(Factory::GenExplosion("big", enemy_list[i].getX(), enemy_list[i].getY()));

                    //移除相应的子弹和敌机
                    enemy_list.removeAt(i);
                    bullet_list.removeAt(j);
                }
                else //敌机血量不为0，则仅移除射中的子弹
                {
                    bullet_list.removeAt(j);
                }
                break; //子弹已被消耗，跳出该层循环
            }
        }
    }

}

void Game_Logic::is_Reward_Get()  //道具获取检测
{
    for(int i = 0; i < reward_list.size(); i++)  //遍历所有奖励道具
    {
        //若玩家飞机和道具的横纵坐标差值的绝对值 均小于 两者对应长/宽的平均值，则认为获得该道具
        if(qAbs(plane.getX() - reward_list[i].getX()) < (plane.getWidth() + reward_list[i].getWidth()) / 2 && qAbs(plane.getY() -reward_list[i].getY()) < (plane.getHeight() + reward_list[i].getHeight()) / 2)
        {
            if(reward_list[i].getName() == "bullet_add")
            {
                double_bullet_stnTime += reward_list[i].getStnTimes();  //双弹时间增加
            }
            else if(reward_list[i].getName() == "boom_add")
            {
                boom_count++;  //炸弹数量增加
            }
            reward_list.removeAt(i);  //移除被获取的道具
        }
    }
}

bool Game_Logic::is_Plane_Down()  //玩家飞机碰撞检测
{
    for(int i = 0; i < enemy_list.size(); i++)  //遍历所有敌机
    {
        //若玩家飞机和敌方飞机的横纵坐标差值的绝对值 均小于 两者对应长/宽的平均值，则认为玩家飞机与敌机发生碰撞
        if(qAbs(plane.getX() - enemy_list[i].getX()) < (plane.getWidth() / 4 + enemy_list[i].getWidth()) / 2 && qAbs(plane.getY() -enemy_list[i].getY()) < (plane.getHeight() - 30 + enemy_list[i].getHeight()) / 2)
        {
            explosion_list.push_back(Factory::GenExplosion("plane", plane.getX(), plane.getY()));
            b_GameOver = true;
            if(score > BestScore)  //若分数超过历史最高，则更新
                UpdateBestScore();
            return true;
        }
    }
    return false;
}

int Game_Logic::getPlaneX()
{
    return plane.getX();
}

int Game_Logic::getPlaneY()
{
    return plane.getY();
}

int Game_Logic::getPlaneWidth()
{
    return plane.getWidth();
}

int Game_Logic::getPlaneHeight()
{
    return plane.getHeight();
}

QImage Game_Logic::getPlaneImage()
{
    return plane.getImage();
}

void Game_Logic::setPlaneX(int x)
{
    plane.setX(x);
}

void Game_Logic::setPlaney(int y)
{
    plane.setY(y);
}

int Game_Logic::get_BulletList_Size()
{
    return bullet_list.size();
}

QString Game_Logic::getBulletListName(int i)
{
    return bullet_list[i].getName();
}

int Game_Logic::getBulletListX(int i)
{
    return bullet_list[i].getX();
}

int Game_Logic::getBulletListY(int i)
{
    return bullet_list[i].getY();
}

int Game_Logic::getBulletListWidth(int i)
{
    return bullet_list[i].getWidth();
}

int Game_Logic::getBulletListHeight(int i)
{
    return bullet_list[i].getHeight();
}

QImage Game_Logic::getBulletListImage(int i)
{
    return bullet_list[i].getImage();
}

void Game_Logic::BulletListPush(Bullet New)
{
    bullet_list.push_back(New);
}

void Game_Logic::BulletListClear()
{
    bullet_list.clear();
}

int Game_Logic::get_EnemyList_Size()
{
    return enemy_list.size();
}

QString Game_Logic::getEnemyListName(int i)
{
    return enemy_list[i].getName();
}

int Game_Logic::getEnemyListX(int i)
{
    return enemy_list[i].getX();
}

int Game_Logic::getEnemyListY(int i)
{
    return enemy_list[i].getY();
}

int Game_Logic::getEnemyListWidth(int i)
{
    return enemy_list[i].getWidth();
}

int Game_Logic::getEnemyListHeight(int i)
{
    return enemy_list[i].getHeight();
}

QImage Game_Logic::getEnemyListImage(int i)
{
    return enemy_list[i].getImage();
}

void Game_Logic::EnemyListPush(Enemy New)
{
    enemy_list.push_back(New);
}

void Game_Logic::EnemyListClear()
{
    enemy_list.clear();
}

int Game_Logic::get_RewardList_Size()
{
    return reward_list.size();
}

QString Game_Logic::getRewardListName(int i)
{
    return reward_list[i].getName();
}

int Game_Logic::getRewardListX(int i)
{
    return reward_list[i].getX();
}

int Game_Logic::getRewardListY(int i)
{
    return reward_list[i].getY();
}

int Game_Logic::getRewardListWidth(int i)
{
    return reward_list[i].getWidth();
}

int Game_Logic::getRewardListHeight(int i)
{
    return reward_list[i].getHeight();
}

QImage Game_Logic::getRewardListImage(int i)
{
    return reward_list[i].getImage();
}

void Game_Logic::RewardListPush(Reward New)
{
    reward_list.push_back(New);
}

void Game_Logic::RewardListClear()
{
    reward_list.clear();
}

int Game_Logic::get_ExplosionList_Size()
{
    return explosion_list.size();
}

int Game_Logic::getExplosionListX(int i)
{
    return explosion_list[i].getX();
}

int Game_Logic::getExplosionListY(int i)
{
    return explosion_list[i].getY();
}

int Game_Logic::getExplosionListWidth(int i)
{
    return explosion_list[i].getWidth();
}

int Game_Logic::getExplosionListHeight(int i)
{
    return explosion_list[i].getHeight();
}

int Game_Logic::getExplosionListCounter(int i)
{
    return explosion_list[i].getCounter();
}

int Game_Logic::getExplosionListStnTimes(int i)
{
    return explosion_list[i].getStnTimes();
}

int Game_Logic::get_ExplosionList_ImagesSize(int i)
{
    return explosion_list[i].getImageSize();
}

QImage Game_Logic::getExplosionListImage(int i, int j)
{
    return explosion_list[i].getImage(j);
}

void Game_Logic::setExplosionListCounter(int i, int counter)
{
    explosion_list[i].setCounter(counter);
}

void Game_Logic::removeExplosionList(int i)
{
    explosion_list.removeAt(i);
}

int Game_Logic::getBestScore()
{
    return BestScore;
}

int Game_Logic::getWinCondition()
{
    return Win_Condition;
}

int Game_Logic::getScore()
{
    return score;
}

int Game_Logic::getBoomCount()
{
    return boom_count;
}

int Game_Logic::getInitBoomCount()
{
    return Init_BoomCount;
}

int Game_Logic::getGameDiffi()
{
    return GameDiffi;
}

int Game_Logic::getDSTime()
{
    return double_bullet_stnTime;
}

int Game_Logic::getScoreBig()
{
    return Score_BigEnemy;
}

int Game_Logic::getScoreMid()
{
    return Score_MidEnemy;
}

int Game_Logic::getScoreSmall()
{
    return Score_SmallEnemy;
}

int Game_Logic::getKeySpeed()
{
    return Plane_Key_Speed;
}

bool Game_Logic::is_GameOver()
{
    return b_GameOver;
}

bool Game_Logic::is_Pause()
{
    return pause;
}

bool Game_Logic::is_Win()
{
    return b_Win;
}

bool Game_Logic::is_Start()
{
    return b_GameStart;
}

bool Game_Logic::is_Music_On()
{
    return b_Music_On;
}

void Game_Logic::GameStart()
{
    b_GameStart = true;
}

void Game_Logic::setWinCondition(int winconditon)
{
    Win_Condition = winconditon;
}

void Game_Logic::setInitBoomCount(int InitBoomCount)
{
    Init_BoomCount = InitBoomCount;
}

void Game_Logic::setBoomCount(int BoomCount)
{
    boom_count = BoomCount;
}

void Game_Logic::setGameDiffi(int Diffi)
{
    GameDiffi = Diffi;
}

void Game_Logic::setMusicOn(bool on_off)
{
    b_Music_On = on_off;
}

void Game_Logic::setScore(int Score)
{
    score = Score;
}

void Game_Logic::setDoubleShootTime(int DST)
{
    double_bullet_stnTime = DST;
}

void Game_Logic::UpdateBestScore()
{
    QFile My_File("./BestScore.txt");
    My_File.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&My_File);
    out << score;
    My_File.close();
    BestScore = score;
}

void Game_Logic::Review_UseBoom()
{
    boom_count--;
    for(int i = 0; i < enemy_list.size(); i++)  //遍历所有敌机
    {
        //玩家得分
        if (enemy_list[i].getName() == "small") score += Score_SmallEnemy;
        else if (enemy_list[i].getName() == "mid") score += Score_MidEnemy;
        else if (enemy_list[i].getName() == "big") score += Score_BigEnemy;

        //爆炸效果
        if(enemy_list[i].getName() == "small")
            explosion_list.push_back(Factory::GenExplosion("small", enemy_list[i].getX(), enemy_list[i].getY()));
        else if(enemy_list[i].getName() == "mid")
            explosion_list.push_back(Factory::GenExplosion("mid", enemy_list[i].getX(), enemy_list[i].getY()));
        else if (enemy_list[i].getName() == "big")
            explosion_list.push_back(Factory::GenExplosion("big", enemy_list[i].getX(), enemy_list[i].getY()));
    }
    enemy_list.clear();  //直接清空敌机列表
    if(score >= Win_Condition && Win_Condition > 0)  //若在非无限模式下达到胜利条件
    {
        b_Win = true;
        b_GameOver = true;
    }
}
