#ifndef MT01_H
#define MT01_H
#include "tower.h"
#include "config.h"
#include "mt01attack.h"

class Mt01 : public Tower
{
public:
    Mt01();


    void shoot(Enemy* target) override;
    void losehealth(int damage) override;
    int getChildStatic() const override;
    void Remove() override;
    void TowerUpdate() override;
    void Update_Image() override;

    void Update_linear_shootrange();
    static int Tower_num;

    //ellipse shoot range
    bool display_shootrange;

    Mt01Attack m_Mt01Attacks[MT01_MAGIC_NUM];


};




#endif // MT01_H
