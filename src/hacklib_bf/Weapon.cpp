#include "Weapon.h"


const void *Weapon::getInternalRep() const
{
    return m_internalRep;
}


float Weapon::getBulletSpeed() const
{
    return m_bulletSpeed;
}

float Weapon::getGravity() const
{
    return m_gravity;
}
