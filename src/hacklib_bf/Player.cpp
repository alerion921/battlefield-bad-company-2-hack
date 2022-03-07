#include "Player.h"


const void *Player::getInternalRep() const
{
    return m_internalRep;
}

bool Player::isValid() const
{
    return m_internalRep != nullptr;
}


std::string Player::getName() const
{
    return m_name;
}

float Player::getHealth() const
{
    return m_health;
}

int Player::getTeamId() const
{
    return m_teamId;
}


D3DXVECTOR3 Player::getPos() const
{
    return m_pos;
}

D3DXVECTOR3 Player::getVel() const
{
    return m_vel;
}

float Player::getRotation() const
{
    return m_rotation;
}


const Skeleton *Player::getSkeleton() const
{
    if (m_skeleton.m_internalRep)
        return &m_skeleton;
    return nullptr;
}
