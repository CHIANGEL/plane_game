#include "factory.h"
#include "bullet.h"
#include "enemy.h"
#include "plane.h"
#include "reward.h"
#include "explosion.h"
#include <QPainter>

Plane Factory::GenPlane()
{
    QImage Image = QImage(":images/images/me1.png");
    return Plane("plane", 250, 500, Image.width(), Image.height(), 0, 0, Image);
}

Enemy Factory::GenEnemy(QString kind, int speedBase, int width,int GameDiffi)
{
    int SpeedX;
    if(kind == "small")
    {
        QImage Image = QImage(":images/images/enemy1.png");

        if(GameDiffi == 0)
            SpeedX = 0;
        else
            SpeedX = qrand()%5-2;

        return Enemy("small", qrand()%width, 0, Image.width(), Image.height(), SpeedX, qrand()%4+2+speedBase, 1, Image);
    }
    else if(kind == "mid")
    {
        QImage Image = QImage(":images/images/enemy2.png");

        if(GameDiffi == 0)
            SpeedX = 0;
        else
            SpeedX = qrand()%5-2;

        return Enemy("mid", qrand()%width, 0, Image.width(), Image.height(), SpeedX, qrand()%4+1+speedBase, 5, Image);
    }
    else if(kind == "big")
    {
        QImage Image = QImage(":images/images/enemy3_n1.png");

        if(GameDiffi == 0)
            SpeedX = 0;
        else
            SpeedX = qrand()%3-1;

        return Enemy("big", qrand()%width, 0, Image.width(), Image.height(), SpeedX, qrand()%3+1+speedBase, 20, Image);
    }
    return Enemy("", 0, 0, 0, 0, 0, 0, 0, QImage());
}

Enemy Factory::GenEnemy(QString kind, int x, int y ,int speedx, int speedy, int hp)
{
    QImage Image;
    if(kind == "small")
    {
        Image = QImage(":images/images/enemy1.png");
    }
    else if(kind == "mid")
    {
        Image = QImage(":images/images/enemy2.png");
    }
    else if(kind == "big")
    {
        Image = QImage(":images/images/enemy3_n1.png");
    }
    return Enemy(kind, x, y, Image.width(), Image.height(), speedx, speedy, hp, Image);
}

Bullet Factory::GenBullet(QString kind,int X,int Y)
{
    if (kind == "red")
    {
        QImage Image = QImage(":images/images/bullet1.png");
        return Bullet("red", X, Y, Image.width(), Image.height(), 0, 20, Image);
    }
    else if (kind == "blue")
    {
        QImage Image = QImage(":images/images/bullet2.png");
        return Bullet("blue", X, Y, Image.width(), Image.height(), 0, 20, Image);
    }
    return Bullet("", 0, 0, 0, 0, 0, 0, QImage());
}

Reward Factory::GenReward(QString kind, int X, int Y)
{
    if (kind == "bullet_add")
    {
        QImage Image = QImage(":images/images/bullet_supply.png");
        return Reward("bullet_add", X, Y, Image.width(), Image.height(), qrand()%5-2, 3, 5000, Image);
    }
    else if (kind == "boom_add")
    {
        QImage Image = QImage(":images/images/bomb_supply.png");
        return Reward("boom_add", X, Y, Image.width(), Image.height(), qrand()%5-2, 3, 5000, Image);
    }
    return Reward("", 0, 0, 0, 0, 0, 0, 0, QImage());
}

Reward Factory::GenReward(QString kind, int x, int y, int speedx, int speedy, int stntimes)
{
    QImage Image;
    if(kind == "bullet_add")
    {
        Image = QImage(":images/images/bullet_supply.png");
    }
    else if(kind == "boom_add")
    {
        Image = QImage(":images/images/bomb_supply.png");
    }
    return Reward(kind, x, y, Image.width(), Image.height(), speedx, speedy, stntimes, Image);
}

QImage Factory::GetBoomIcon()
{
    return QImage(":images/images/bomb.png");
}

Explosion Factory::GenExplosion(QString kind, int X, int Y)
{
    QList<QImage> Images;
    if(kind == "small")
    {
        Images.append(QImage(":images/images/enemy1_down1.png"));
        Images.append(QImage(":images/images/enemy1_down2.png"));
        Images.append(QImage(":images/images/enemy1_down3.png"));
        Images.append(QImage(":images/images/enemy1_down4.png"));
        return Explosion(X, Y, 300, "small", Images);
    }
    else if(kind == "mid")
    {
        Images.append(QImage(":images/images/enemy2_down1.png"));
        Images.append(QImage(":images/images/enemy2_down2.png"));
        Images.append(QImage(":images/images/enemy2_down3.png"));
        Images.append(QImage(":images/images/enemy2_down4.png"));
        return Explosion(X, Y, 300, "mid", Images);
    }
    else if(kind == "big")
    {
        Images.append(QImage(":images/images/enemy3_down1.png"));
        Images.append(QImage(":images/images/enemy3_down2.png"));
        Images.append(QImage(":images/images/enemy3_down3.png"));
        Images.append(QImage(":images/images/enemy3_down4.png"));
        Images.append(QImage(":images/images/enemy3_down5.png"));
        Images.append(QImage(":images/images/enemy3_down6.png"));
        return Explosion(X, Y, 500, "big", Images);
    }
    else if(kind == "plane")
    {
        Images.append(QImage(":images/images/me_destroy_1.png"));
        Images.append(QImage(":images/images/me_destroy_2.png"));
        Images.append(QImage(":images/images/me_destroy_3.png"));
        Images.append(QImage(":images/images/me_destroy_4.png"));
        return Explosion(X, Y, 300, "plane", Images);
    }
    return Explosion(0, 0, 0, "", Images);
}
