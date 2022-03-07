#ifndef PLAYER_H
#define PLAYER_H

#include "Skeleton.h"
#include <string>


class Player
{
    friend class BattlefieldDataMgr;

public:
    const void *getInternalRep() const;
    bool isValid() const;

    std::string getName() const;
    float getHealth() const;
    int getTeamId() const;

    D3DXVECTOR3 getPos() const;
    D3DXVECTOR3 getVel() const;
    float getRotation() const;

    const Skeleton *getSkeleton() const;

private:
    void *m_internalRep = nullptr;

    std::string m_name;
    float m_health = 0;
    int m_teamId = -1;

    D3DXVECTOR3 m_pos = D3DXVECTOR3(0, 0, 0);
    D3DXVECTOR3 m_vel = D3DXVECTOR3(0, 0, 0);
    float m_rotation = 0;

    Skeleton m_skeleton;

};

#endif
