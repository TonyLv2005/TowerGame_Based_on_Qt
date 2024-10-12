#ifndef DEATH_H
#define DEATH_H
#include <QPixmap>
#include "config.h"


class Death
{
public:
    Death();

    virtual void Image_update() = 0;
    QVector<QPixmap> m_images;

    int pix_x;
    int pix_y;

    bool m_Free;
    int image_recorder;
    int image_index;

};

#endif // DEATH_H
