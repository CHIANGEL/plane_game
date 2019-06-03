#ifndef EXPLOSION_H
#define EXPLOSION_H

#include <QList>
#include <QPainter>

/*
爆炸效果类，内部用QList建立一组Images，进而形成动画效果
*/

class Explosion
{
public:
    Explosion(int X, int Y, int StnTimes, QList<QImage> Images);
    int getX();
    int getY();
    int getWidth();
    int getHeight();
    int getCounter();
    int getStnTimes();
    int getImageSize();
    QImage getImage(int i);
    void setCounter(int counter);

private:
    int X;
    int Y;
    int Width;
    int Height;
    int StnTimes;
    int Counter;
    QList<QImage> Images;

};
#endif // EXPLOSION_H
