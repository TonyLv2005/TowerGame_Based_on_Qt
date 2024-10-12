#include "mt01.h"
#include "enemy.h"

int Mt01::Tower_num = MT01_NUM;

Mt01::Mt01()
{
    max_hp = MT01_HP;
    hp = max_hp;
    cost = MT01_COST;

    //Identity = "Mt01";
    Identity = MT01_IDENTITY;

    for(Mt01Attack& temp: m_Mt01Attacks){
        temp = Mt01Attack();
    }

    max_damage = MT01_DAMAGE;
    damage = max_damage;
    max_shootrange = MT01_SHOOTRANGE;
    shootrange = max_shootrange;
    max_interval = MT01_MAGIC_INTERVAL;
    m_interval = max_interval;
    m_recorder = m_interval;

    //towers load
    for(int i = 1; i <= MT01_IMAGE_ATTACK_MAX ; i++){
        QString str = QString(MT01_PATH).arg(i);
        m_Towers.push_back(QPixmap(str));
    }

    barWidth = m_Towers[0].width()/2;
    display_shootrange = false;


    //set default hitbox
    m_Rect.setWidth(m_Towers[0].width());
    m_Rect.setHeight(m_Towers[0].height());
    //m_Rect.moveTo(m_X,m_Y);

}

void Mt01::shoot(Enemy* target)
{
    m_recorder++;
    if(m_recorder < m_interval)
        return;

    //SHOOTING
    target->losehealth(damage);
    //Shoot display
    image_attack_enable = true;
    image_attack_recorder = 0;
    image_attack_index = 0;
    //consist_lose health
    //qDebug() << "mt01 set the health";
    if(consist_damage_available){
        target->m_consist_losehealth = true;
        target->m_consist_total = 0;
    }
    //freeze
    if(freeze_available){
        target->m_freeze = true;
        target->m_freeze_recorder = 0;
    }


    m_recorder = 0;

    //media
    Player_magic->play();
    //display the attack

    for(int i =0 ; i < MT01_MAGIC_NUM ; i++){
        if(m_Mt01Attacks[i].m_Free){
            m_Mt01Attacks[i].m_Free = false;
            m_Mt01Attacks[i].m_Rect.moveTo(target->m_Rect.x(),target->m_Rect.y());
            break;
        }
    }


    if(target->m_Free){
        //buff drop
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> dis(0,3);


        int choice = dis(gen);
        if(choice == 0){
            buff_freeze_collect = true;
        }
        if(choice == 1){
            buff_bleed_collect = true;
        }
        if(choice == 2){
            buff_widerange_collect = true;
        }
        if(choice == 3){
            buff_speedup_collect = true;
        }
    }

}

void Mt01::losehealth(int damage)
{
    // to be done
    hp -= damage;

    if(hp <= 0){
        Remove();
    }
}


int Mt01::getChildStatic() const
{
    return Tower_num;
}

void Mt01::Remove()
{
    hp = MT01_HP;
    m_X = 0;
    m_Y = 0;
    m_Rect.moveTo(m_X,m_Y);
    m_Free = true;
    freeze_available = false;
    consist_damage_available = false;
    speedup_available = false;
    widerange_available = false;

    //reset buff collect
    buff_bleed_collect = false;
    buff_freeze_collect = false;
}

void Mt01::TowerUpdate()
{
    if(m_Free){
        return;
    }

    for(Mt01Attack& at : m_Mt01Attacks){
        if(!at.m_Free)
            at.Update_Image();
    }

    Update_Image();
    Update_linear_shootrange();
}

void Mt01::Update_Image()
{
    if(m_Free || !image_attack_enable){
        return;
    }
    image_attack_recorder++;
    if(image_attack_recorder < MT01_IMAGE_ATTACK_INTERVAL){
        return;
    }
    image_attack_recorder = 0;

    // wait for interval to update the image_index

    image_attack_index++;

    if(image_attack_index > MT01_IMAGE_ATTACK_MAX - 1){
        image_attack_index = 0;
        image_attack_enable = false;
    }
}

void Mt01::Update_linear_shootrange()
{
    if(widerange_available){
        if(shootrange < max_shootrange * MT01_SHOOTRANGE_COEFFICIENT){
            shootrange += 3;
        }
    }else{
        if(shootrange > max_shootrange){
            shootrange -= 3;
        }
    }
}



