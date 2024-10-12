#include "mt01attack.h"
#include <QDebug>

Mt01Attack::Mt01Attack()
{
    for(int i = 1; i <= MT01_MAGIC_IMAGE_MAX ; i++){
        QString str = QString(MT01_MAGIC_PATH).arg(i);
        m_magics.push_back(QPixmap(str));
    }

    //set default hitbox
    m_Rect.setWidth(m_magics[0].width());
    m_Rect.setHeight(m_magics[0].height());
    m_Rect.moveTo(0,0);

}

int Mt01Attack::getChildStatic() const
{
    return MT01_MAGIC_NUM;
}

int Mt01Attack::getChildInterval() const
{
    return MT01_MAGIC_INTERVAL;
}

void Mt01Attack::Update_Image()
{

    if(m_Free){
        return;
    }
    image_recorder++;
    if(image_recorder < MT01_IMAGE_ATTACK_INTERVAL){
        return;
    }
    image_recorder = 0;

    // wait for interval to update the image_index

    image_index++;

    if(image_index > MT01_MAGIC_IMAGE_MAX - 1){
        image_index = 0;
        m_Free = true;
    }

}




