#ifndef MUSICPLAY_H
#define MUSICPLAY_H

#include <QSound>

/*
音乐播放类
*/

class MusicPlay
{
public:
    MusicPlay(QString MusicPath);
    ~MusicPlay();
    void SetInfinite();  //设置音乐循环播放
    void Play();
    void Stop();

private:
    QString musicPath;
    QSound* bells;

};

#endif // MUSICPLAY_H
