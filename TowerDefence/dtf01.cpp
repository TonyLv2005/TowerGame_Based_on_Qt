#include "dtf01.h"
#include "enemy.h"

Dtf01::Dtf01()
{
    max_hp = DTF01_HP;
    hp = max_hp;
    cost = DTF01_COST;

    for(int i = 1; i <= DTF01_IMAGE_ATTACK_MAX ; i++){
        QString str = QString(DTF01_IMAGE_LEFT_PATH).arg(i);
        m_Towers.push_back(QPixmap(str));
    }

    //Identity = "Dtf01";
    Identity = DTF01_IDENTITY;

    damage = DTF01_DAMAGE;
    shootrange = DTF01_SHOOTRANGE;
    m_interval = DTF01_ATTACK_INTERVAL;
    m_recorder = m_interval;

    barWidth = m_Towers[0].width();

    //set default hitbox
    m_Rect.setWidth(m_Towers[0].width());
    m_Rect.setHeight(m_Towers[0].height());
    //m_Rect.moveTo(m_X,m_Y);
}

void Dtf01::shoot(Enemy* target)
{
    m_recorder++;
    if(m_recorder < DTF01_ATTACK_INTERVAL)
        return;

    //SHOOTING
    Player_knife->play();
    //shooting display
    image_attack_enable = true;
    image_attack_index = 0;
    image_attack_recorder = 0;
    //freeze
    /*
    if(freeze_available){
        target->m_freeze = true;
        target->m_freeze_recorder = 0;
    }
*/


    target->losehealth(damage);
    m_recorder = 0;
    //qDebug() << "dtf01 cause damage";

    if(target->m_Free){
        //buff drop
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> dis(0,2);

        int choice = dis(gen);
        if(choice == 0){
            buff_craze_collect = true;
        }
        if(choice == 1){
            buff_freeze_collect = true;
        }
        if(choice == 2){
            buff_aoe_collect = true;
        }
    }
    //display the attack

}

void Dtf01::losehealth(int damage)
{
    // to be done
    hp -= damage;

    if(hp <= 0){
        Player_dead->play();
        Remove();
    }
}

int Dtf01::getChildStatic() const
{
    return DTF01_NUM;
}

void Dtf01::Remove()
{
    hp = DTF01_HP;
    m_X = 0;
    m_Y = 0;
    m_Rect.moveTo(m_X,m_Y);
    m_Free = true;

    //reset buff collect
    buff_freeze_collect = false;
    buff_craze_collect = false;
    buff_aoe_collect = false;
}

void Dtf01::TowerUpdate()
{
    if(m_Free){
        return;
    }
    Update_Image();
    updateBuffInfo();
}

void Dtf01::Update_Image()
{
    if(m_Free || !image_attack_enable){
        return;
    }
    image_attack_recorder++;
    if(image_attack_recorder < DTF01_IMAGE_ATTACK_INTERVAL){
        return;
    }
    image_attack_recorder = 0;

    // wait for interval to update the image_index

    image_attack_index++;

    if(image_attack_index > DTF01_IMAGE_ATTACK_MAX - 1){
        image_attack_index = 0;
        image_attack_enable = false;
    }
}

void Dtf01::updateBuffInfo()
{
    //craze
    if(craze_available){
        m_interval = DTF01_ATTACK_INTERVAL * 2;
        damage = DTF01_DAMAGE * 2;
    }
    else{
        m_interval = DTF01_ATTACK_INTERVAL;
        damage = DTF01_DAMAGE;
    }
}
