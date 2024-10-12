#include "bs01.h"
#include "tower.h"

int Bs01::Enemy_num = BS01_NUM;

Bs01::Bs01()
{

    max_hp = BS01_HP;
    hp = max_hp;
    EnemyState = BS01_STATE_MOVE;
    destroy_gold_credit = BS01_DESTROY_GOLD_CREDIT;

    //buff initialize
    buff_fast_collect = false;
    fast_available = false;
    buff_sprint_collect = false;

    //Enemys load
    for(int i = 1; i <= BS01_IMAGE_MOVE_MAX ; i++){
        QString str = QString(BS01_PATH).arg(i,2,10,QChar('0'));
        m_Enemy_move.push_back(QPixmap(str));
    }
    for(int i = 1; i <= BS01_IMAGE_ATTACK_MAX ; i++){
        QString str = QString(BS01_ATTACK_PATH).arg(i,2,10,QChar('0'));
        m_Enemy_attacks.push_back(QPixmap(str));
    }
    m_Enemys = m_Enemy_move;//initialization

    Identity = BS01_IDENTITY;

    shootrange = BS01_SHOOTRANGE;
    max_speed = BS01_SPEED;
    m_speed = max_speed;

    m_interval = BS01_ATTACK_INTERVAL;
    m_recorder = m_interval;
    m_damage = BS01_DAMAGE;
    //raisehand_interval = BS01_RAISEHAND_INTERVAL;
    barWidth = m_Enemys[0].width()/2;

    //set default hitbox
    m_Rect.setWidth(m_Enemys[0].width());
    m_Rect.setHeight(m_Enemys[0].height());
    m_Rect.moveTo(m_X,m_Y);



}


int Bs01::getChildStatic() const
{
    return Enemy_num;
}


int Bs01::getChildInterval() const
{
    return BS01_INTERVAL;
}


void Bs01::updatePos()
{
    if(m_Free){
        return;
    }

    if(m_freeze){
        m_damage = 0;
    }else{
        m_damage = BS01_DAMAGE;
    }
    if(EnemyState!=BS01_STATE_MOVE and !sprint_available){
        m_speed = 0;
        return ; //skip down below
    }else{
        if(m_freeze){
            m_speed = 0;
            return;
        }
        m_speed = max_speed;
        if(buff_fast_collect){
            m_speed = max_speed + 1;
        }

    }

}

void Bs01::EnemyUpdate()
{
    if(m_Free){
        return;
    }
    updatePos();
    freezed_count();
    consist_losehealth();
    sprint_count();
    Update_Image();
}

void Bs01::Update_Image()
{
    // update image
    if(m_Free){
        return;
    }

    image_recorder++;

    //move
    if(EnemyState == BS01_STATE_MOVE){
        if(image_recorder < BS01_IMAGE_MOVE_INTERVAL){
            return;
        }
        m_Enemys = m_Enemy_move;
        image_recorder = 0;

        image_index++;

        if(image_index >= BS01_IMAGE_MOVE_MAX){
            image_index = 0;
            //image_move_enable = false;
        }
    }
    //attack
    if(EnemyState == BS01_STATE_ATTACK){
        if(image_recorder < BS01_IMAGE_ATTACK_INTERVAL){
            return;
        }

        m_Enemys = m_Enemy_attacks;
        image_recorder = 0;

        image_index++;

        if(image_index >= BS01_IMAGE_ATTACK_MAX){
            image_index = 0;
        }
    }

}



void Bs01::Remove()
{

    m_Free = true;
    EnemyState = BS01_STATE_MOVE;
    m_Enemys = m_Enemy_move;
    m_X = 0;
    m_Y = 0;
    hp = BS01_HP;

    //m_Rect.moveTo(m_X,m_Y);
    HaveDone.clear();

    //reset the buff collect
    buff_fast_collect = false;
    buff_sprint_collect = false;
}

void Bs01::shoot(Tower* target)
{
    m_recorder++;
    if(m_recorder < BS01_ATTACK_INTERVAL)
        return;
    //SHOOTING
    //m_stop = true;
    //raisehand_recorder ++;
    //stop move first and raise hand // animation wait to realize
    //if(raisehand_recorder < raisehand_interval)
     //   return;
    //qDebug() << "bs01 cause damage";
    target->losehealth(m_damage);
    m_recorder = 0;
    //m_stop = false;
    //raisehand_recorder = 0;
    //display the attack

    //media
    Player_knife->play();

    if(target->m_Free){
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0,1);

        int choice = dis(gen);
        if(choice == 0){
            buff_fast_collect = true;
        }
        if(choice == 1){
            buff_sprint_collect = true;
        }
        //qDebug() << destroyed;
    }

}

void Bs01::losehealth(int damage)
{
    // to be done
    hp -= damage;

    if(hp<= 0){
        Player_dead->play();
        Remove();
    }

}



/*喜欢entp!
     * 喜欢！
     * 特别喜欢！*/ //我到时候就把这个交上去
//怎么可以讨厌entp 你看你把讨厌entp打在这里 代码就运行不了了 所以不可以讨厌entp TuT
//注释了就能跑了
