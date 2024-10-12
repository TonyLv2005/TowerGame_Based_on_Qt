#ifndef BS01_DEATH_H
#define BS01_DEATH_H
#include "config.h"
#include <QPixmap>
#include <QVector>
//#include "death.h"


class Bs01_death
{
public:
    Bs01_death();

    void Image_update() ;

    QVector<QPixmap> m_images;

    int pix_x;
    int pix_y;

    bool m_Free;
    int image_recorder;
    int image_index;
};

#endif // BS01_DEATH_H
