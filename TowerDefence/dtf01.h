#ifndef DTF01_H
#define DTF01_H
#include "tower.h"

class Dtf01 : public Tower
{
public:
    Dtf01();



    void shoot(Enemy* target) override;
    void losehealth(int damage) override;
    int getChildStatic() const override;
    void Remove() override;
    static int Tower_num;
    void TowerUpdate() override;
    void Update_Image() override;
    void updateBuffInfo();

};

#endif // DTF01_H
