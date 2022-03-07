#ifndef BATTLEFIELDDATA_H
#define BATTLEFIELDDATA_H

#include "Player.h"
#include "CamData.h"
#include "OwnData.h"
#include "Weapon.h"
#include <vector>


class BattlefieldData
{
    friend class BattlefieldDataMgr;

public:
    const void *getInternalRep() const;

    bool isIngame() const;
    const std::vector<Player> &getPlayers() const;
    const Player *getOwnPlayer() const;
    const CamData *getCamData() const;
    const OwnData *getOwnData() const;
    const Weapon *getWeapon() const;

private:
    void *m_internalRep = nullptr;

    bool m_isIngame = false;
    std::vector<Player> m_players;
    size_t m_ownPlayerIndex = -1;
    CamData m_camData;
    OwnData m_ownData;
    Weapon m_weapon;

};

#endif
