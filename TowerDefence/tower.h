#ifndef TOWER_H
#define TOWER_H
#include "config.h"
#include <QPixmap>
#include <QRect>
#include <QVector>
#include <QString>
#include <random>
#include "bullet.h"
#include <QMediaPlayer>
#include <QAudioOutput>

class Enemy;

class Tower
{
public:
    Tower();

    int hp;
    int max_hp;
    int cost;
    bool m_Free;
    int max_damage;
    int damage;
    int max_shootrange;
    int shootrange;
    //QString Identity;
    int Identity;
    //image
    int image_attack_recorder;
    int image_attack_index;
    bool image_attack_enable;

    //update
    virtual void TowerUpdate() = 0;
    virtual void Update_Image() = 0;

    //buff collect
    bool buff_bleed_collect;
    bool buff_freeze_collect;
    bool buff_craze_collect;
    bool buff_aoe_collect;
    bool buff_widerange_collect;
    bool buff_speedup_collect;

    QVector<QPixmap> m_Towers;

    virtual void shoot(Enemy* target) = 0;
    //get the static member of child class
    virtual int getChildStatic() const = 0;
    virtual void losehealth(int damage) = 0;
    virtual void Remove() = 0;
    void setPosition(int x, int y);

    //cordinate
    int m_X;
    int m_Y;

    //hitbox
    QRect m_Rect;

    //attack interval control
    int m_recorder;
    int m_interval;
    int max_interval;

    //buff
    bool freeze_available;
    bool consist_damage_available;
    bool craze_available;
    void updateBuffInfo();
    bool widerange_available;
    bool speedup_available;
    bool aoe_available;

    //HP bar
    int barWidth;
    int barHeight;

    //media
    QMediaPlayer* Player_magic;
    QAudioOutput* AuOut_magic;
    QMediaPlayer* Player_knife;
    QAudioOutput *AuOut_knife;
    QMediaPlayer* Player_dead;
    QAudioOutput *AuOut_dead;
    QMediaPlayer* Player_buff;
    QAudioOutput *AuOut_buff;



};

#endif // TOWER_H
