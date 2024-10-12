#ifndef BULLET_H
#define BULLET_H
#include "config.h"
#include <QPixmap>
#include <QRect>


class Bullet
{
public:
    Bullet();

    virtual void upPosition() = 0;

    QPixmap m_bullet;

    int m_X;
    int m_Y;

    int m_speed;
    bool m_Free;

    //hitbox
    QRect m_Rect;
};

#endif // BULLET_H
