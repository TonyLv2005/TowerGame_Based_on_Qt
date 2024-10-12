#ifndef BS01_H
#define BS01_H
#include "enemy.h"
#include "config.h"

class Bs01 : public Enemy
{
public:
    Bs01();

    //get the static member of child class
    int getChildStatic() const override;
    int getChildInterval() const override;

    void updatePos();
    void EnemyUpdate() override;
    void Update_Image() override;
    void Remove() override;
    static int Enemy_num;
    void shoot(Tower* target) override;
    void losehealth(int damage) override;


};



#endif // BS01_H
