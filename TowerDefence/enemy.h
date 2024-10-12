#ifndef ENEMY_H
#define ENEMY_H
#include "config.h"
#include "PathPoint.h"
#include <QPixmap>
#include <QRect>
#include <QVector>
#include <random>
#include <QProgressBar>
#include <QDebug>
#include <QMediaPlayer>
#include <QAudioOutput>

class Tower;

class Enemy
{
public:
    Enemy();

    int hp;
    int m_damage;
    int max_hp;
    int destroy_gold_credit;

    //cordinate
    int m_X;
    int m_Y;

    int Identity;
    QVector<QPixmap> m_Enemys;
    QVector<QPixmap> m_Enemy_move;
    QVector<QPixmap> m_Enemy_attacks;
    int EnemyState;

    virtual void shoot(Tower* target) = 0;
    virtual void EnemyUpdate() = 0;
    virtual void Update_Image() = 0;

    //get the static member of child class
    virtual int getChildStatic() const = 0;
    virtual int getChildInterval() const = 0;
    //remove when delete
    virtual void Remove() = 0;

    int max_speed;
    int m_speed;

    bool m_Free;
    PathPoint target = PathPoint(0,0);

    //image
    int image_recorder;
    int image_index;

    //HP bar
    int barWidth; //hardcode
    int barHeight;

    virtual void losehealth(int damage) = 0;

    //Logic Position
    //PathPoint m_pos;
    QVector<PathPoint> HaveDone;


    QRect m_Rect;

    //shootrange
    int shootrange;
    //int raisehand_recorder;
    //int raisehand_interval;
    //attack interval
    int m_interval;
    int m_recorder;

    //buff
    //negative to enemy
    //consistent lose health
    bool m_consist_losehealth;
    int m_consist_recorder;
    int m_consist_total ;
    void consist_losehealth();
    //freeze
    bool m_freeze;
    int m_freeze_recorder;
    void freezed_count();
    //fast
    bool fast_available;
    void addBuff_fast();
    //Sprint
    bool sprint_available;
    void addBuff_Sprint();
    int m_sprint_recorder;
    //void sprint_count();

    //buff collect
    bool buff_fast_collect;
    bool buff_sprint_collect;

    //positive to enemy
    bool m_sprint;
    void sprint_count();

    //Media
    QMediaPlayer* Player_knife;
    QAudioOutput *AuOut_knife;

    QMediaPlayer* Player_dead;
    QAudioOutput *AuOut_dead;

    QMediaPlayer* Player_buff;
    QAudioOutput *AuOut_buff;

};



#endif // ENEMY_H
