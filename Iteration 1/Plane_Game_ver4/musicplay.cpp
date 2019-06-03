#include "musicplay.h"
#include <QSound>

MusicPlay::MusicPlay(QString musicPath)
{
    this->musicPath = musicPath;
    bells = new QSound(musicPath);
}

MusicPlay::~MusicPlay()
{
    delete bells;
}

void MusicPlay::SetInfinite()
{
    bells->setLoops(QSound::Infinite);
}

void MusicPlay::Play()
{
    bells->setLoops(QSound::Infinite);
    bells->play();
}

void MusicPlay::Stop()
{
    bells->stop();
}
