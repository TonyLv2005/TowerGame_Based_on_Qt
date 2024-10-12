#include "tower.h"
#include "enemy.h"
Tower::Tower()
{
    m_Free = true;

    //buff
    freeze_available = false;
    craze_available = false;
    aoe_available  = false;
    consist_damage_available = false;
    widerange_available = false;
    speedup_available = false;
    m_X = 0;
    m_Y = 0;
    //hp bar
    barHeight = HEALTH_BAR_HEIGHT;

    image_attack_enable = false;
    image_attack_recorder = 0;
    image_attack_index = 0;

    //buff initialization
    buff_freeze_collect = false;
    buff_craze_collect = false;
    buff_aoe_collect = true;
    buff_bleed_collect = false;
    buff_widerange_collect = false;
    buff_speedup_collect = false;

    //media

    //magic
    Player_magic = new QMediaPlayer(nullptr);
    AuOut_magic = new QAudioOutput(nullptr);
    Player_magic->setSource(QUrl(SOUND_MAGIC_ATTACK_PATH));
    Player_magic->setAudioOutput(AuOut_magic);
    AuOut_magic->setVolume(GAME_VOLUME);

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

void Tower::setPosition(int x, int y)
{
    m_X = x;
    m_Y = y;

    m_Rect.moveTo(m_X,m_Y);
}



void Tower::updateBuffInfo()
{

}




