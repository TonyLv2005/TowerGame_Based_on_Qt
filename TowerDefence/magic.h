#ifndef MAGIC_H
#define MAGIC_H
#include <QPixmap>
#include <QRect>
#include "config.h"

class Magic
{
public:
    Magic();

    QVector<QPixmap> m_magics;
    QRect m_Rect;

    int m_X;
    int m_Y;
    int cnt;
    int image_recorder;
    int image_index;

    bool m_Free;

    //get the static member of child class
    virtual int getChildStatic() const = 0;
    virtual int getChildInterval() const = 0;
    virtual void Update_Image() = 0;


    //virtual void display() = 0;

};

#endif // MAGIC_H
