#ifndef ENTITY_H
#define ENTITY_H

#include <QString>
#include <QPainter>

/*
 * 首先定义一个实体类，作为玩家飞机、敌方飞机、奖励道具等类的基类
 * 包含名称、坐标、长宽、移动速度、图像
 */

class Entity
{
public:
    QString getName();
    int getX();
    int getY();
    int getWidth();
    int getHeight();
    int getSpeedX();
    int getSpeedY();
    QImage getImage();
    void setName(QString Name);
    void setX(int X);
    void setY(int Y);
    void setWidth(int Width);
    void setHeight(int Height);
    void setSpeedX(int SpeedX);
    void setSpeedY(int SpeedY);
    void setImage(QImage Image);

private:
    QString Name;  //对象的名称（种类）
    int X;  //对象横坐标X
    int Y;  //对象纵坐标Y
    int Width;  //对象的宽度
    int Height;  //对象的高度
    int SpeedX;  //对象的水平移动速度
    int SpeedY;  //对象的竖直移动速度
    QImage Image;  //对象的图像
};

#endif // ENTITY_H
