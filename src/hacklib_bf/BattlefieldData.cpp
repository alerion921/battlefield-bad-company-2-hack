#include "BattlefieldData.h"


const void *BattlefieldData::getInternalRep() const
{
    return m_internalRep;
}


bool BattlefieldData::isIngame() const
{
    return m_isIngame;
}

const std::vector<Player> &BattlefieldData::getPlayers() const
{
    return m_players;
}

const Player *BattlefieldData::getOwnPlayer() const
{
    if (m_ownPlayerIndex != -1)
        return &m_players[m_ownPlayerIndex];
    return nullptr;
}

const CamData *BattlefieldData::getCamData() const
{
    if (m_camData.m_internalRep)
        return &m_camData;
    return nullptr;
}

const OwnData *BattlefieldData::getOwnData() const
{
    if (m_ownData.m_internalRep)
        return &m_ownData;
    return nullptr;
}

const Weapon *BattlefieldData::getWeapon() const
{
    if (m_weapon.m_internalRep)
        return &m_weapon;
    return nullptr;
}
