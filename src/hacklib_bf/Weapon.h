#ifndef WEAPON_H
#define WEAPON_H


class Weapon
{
    friend class BattlefieldDataMgr;
    friend class BattlefieldData;

public:
    const void *getInternalRep() const;

    float getBulletSpeed() const;
    float getGravity() const;

private:
    void *m_internalRep = nullptr;

    float m_bulletSpeed = 0;
    float m_gravity = 0;

};

#endif