#include "enemy.h"
#include "tower.h"

Enemy::Enemy()
{
    //the default properties that every enmey has the same
    m_Free = true;
    m_X = 0;
    m_Y = 0;

    fast_available = false;

    //image
    image_recorder = 0;
    image_index = 0;

    //set buff detect
    m_freeze = false;
    m_consist_losehealth = false;
    m_sprint = false;
    sprint_available = false;

    //buff
    m_consist_total = 0;
    m_consist_recorder = 0;
    m_freeze_recorder = 0;
    m_sprint_recorder = 0;

    //hp bar
    barHeight = HEALTH_BAR_HEIGHT;

    //media
    //knife
    Player_knife = new QMediaPlayer(nullptr);
    AuOut_knife = new QAudioOutput(nullptr);
    Player_knife->setSource(QUrl(SOUND_KNIFE_ATTACK_PATH));
    Player_knife->setAudioOutput(AuOut_knife);
    AuOut_knife->setVolume(GAME_VOLUME);
    //dead
    Player_dead = new QMediaPlayer(nullptr);
    AuOut_dead = new QAudioOutput(nullptr);
    Player_dead->setSource(QUrl(SOUND_DEAD_PATH));
    Player_dead->setAudioOutput(AuOut_dead);
    AuOut_dead->setVolume(GAME_VOLUME);
    //buff
    Player_buff = new QMediaPlayer(nullptr);
    AuOut_buff = new QAudioOutput(nullptr);
    Player_buff->setSource(QUrl(SOUND_BUFF_PATH));
    Player_buff->setAudioOutput(AuOut_buff);
    AuOut_buff->setVolume(GAME_VOLUME);

}

void Enemy::consist_losehealth()
{
    if(m_Free or !m_consist_losehealth){
        return ;
    }

    m_consist_recorder++;
    if(m_consist_recorder < BS01_ATTACK_INTERVAL)
        return;

    //SHOOTING

    qDebug() << "bs01 consist lose health";
    losehealth(CONSIST_LOSEHEALTH_DAMAGE); // the losehealth can't be overlap here

    m_consist_recorder = 0;
    m_consist_total ++;

    if(m_consist_total >= CONSIST_LOSEHEALTH_TOTAL){
        m_consist_total = 0;
        m_consist_losehealth = false;
    }


}

void Enemy::freezed_count()
{
    if(!m_freeze)
        return;

    //m_stop = true;
    image_index = 0;

    //freeze

    m_freeze_recorder ++;
    if(m_freeze_recorder >= FREEZE_TOTAL_TIME){
        m_freeze = false;
        m_freeze_recorder = 0;
    }else{
        return;
    }

    //
}

void Enemy::addBuff_fast()
{
    if(m_Free){
        return;
    }

    m_speed = max_speed + 1;
    fast_available = true;
}

void Enemy::addBuff_Sprint()
{
    if(m_Free){
        return;
    }
    m_sprint = true;
    m_sprint_recorder = 0;
    sprint_available = true;
}


void Enemy::sprint_count()
{
    if(!m_sprint)
        return;

    m_speed = max_speed + 2;
    //fast
    m_sprint_recorder ++;

    if(m_sprint_recorder >= SPRINT_FAST_INTERVAL){
        m_sprint = false;
        m_sprint_recorder = 0;
        sprint_available = false;
        m_speed = max_speed;
    }else{
        return;
    }
}

