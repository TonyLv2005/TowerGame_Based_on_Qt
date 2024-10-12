#include "bs01_death.h"

Bs01_death::Bs01_death()
{
    for(int i = 1; i <= BS01_IMAGE_DEAD_MAX ; i++){
        QString str = QString(BS01_DEAD_PATH).arg(i);
        m_images.push_back(QPixmap(str));
    }

}

void Bs01_death::Image_update()
{
    image_recorder ++;

    if(image_recorder < BS01_IMAGE_DEAD_INTERVAL){
        return;
    }


    image_recorder = 0;

    image_index++;

    if(image_index >= BS01_IMAGE_DEAD_MAX){
        image_index = 0;
        m_Free = true;
    }

}
