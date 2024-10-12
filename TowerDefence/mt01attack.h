#ifndef MT01ATTACK_H
#define MT01ATTACK_H
#include "magic.h"

class Mt01Attack : public Magic
{
public:
    Mt01Attack();

    //get the static member of child class
    int getChildStatic() const override;
    int getChildInterval() const override;
    void Update_Image() override;

    //void display() override;
};

#endif // MT01ATTACK_H
