#ifndef BFBC2_MAIN_H
#define BFBC2_MAIN_H

#include "../hacklib/include/hacklib/ImplementMember.h"
#include "BFBC2_Cam.h"
#include "BFBC2_Input.h"
#include "BFBC2_Library.h"
#include "BFBC2_Mystate.h"
#include "BFBC2_Skeleton.h"
#include "BFBC2_Weapon.h"


namespace BFBC2
{
    class ClientSoldierReplication
    {
        IMPLMEMBER(fb::Vec3, Velocity, 0x30);
    };

    class VehicleHealthZoneData
    {
        IMPLMEMBER(float, MaxHealth, 0x1c);
    };
    struct HealthZone
    {
        float health;
        VehicleHealthZoneData *data;
        unsigned int effectHandle;
    };
    class ClientVehicleEntityHealth
    {
        IMPLMEMBER(eastl::vector<HealthZone>, HealthZones, 0x14);
    };

    struct ComponentCollection
    {
        uint32_t _00[3];
        unsigned char playerCount;
        unsigned char totalCount;
        unsigned char offsetCount;
    };
    class ClientControllableEntity
    {
        IMPLMEMBER(ComponentCollection*, Collection, 0x28);
        IMPLMEMBER(float, Health, 0x34);
    };
    class ClientVehicleEntity : public ClientControllableEntity
    {
        IMPLMEMBER(fb::Vec3, Speed, 0x190);
        IMPLMEMBER(ClientVehicleEntityHealth*, Health, 0x1c4);
    };
    class ClientSoldierEntity : public ClientControllableEntity
    {
        IMPLMEMBER(int, KitId, 0x1a8);
        IMPLMEMBER(ClientSoldierReplication*, ReplicatedController, 0x234);
        IMPLMEMBER_REL(ClientSoldierWeapon*, CurrentWeapon, 0x38, ReplicatedController);
        IMPLMEMBER(PersonView, PersonView1, 0x3cc);
        IMPLMEMBER_REL(PersonView, PersonView2, 0, PersonView1);
        IMPLMEMBER(bool, Visible, 0x47a);
        IMPLMEMBER(bool, Shielded, 0x47f); /// VERIFY
        IMPLMEMBER(float, MaxHealth, 0x5a4);
    };

    struct SoldierPtr
    {
        uint32_t *offsettedSoldier;
    };
    class ClientPlayer
    {
        IMPLMEMBER(char*, Name, 0x8);
        IMPLMEMBER(int, TeamId, 0xa8);
        IMPLMEMBER(SoldierPtr, Soldier, 0xc54);
        IMPLMEMBER_REL(ClientControllableEntity*, AttachedControllable, 0x8, Soldier);
    };

    class RayCastHit
    {
    };
    class EntityManager
    {
    public:
        virtual bool physicsRayQuery(const char *ident, fb::Vec3 *from, fb::Vec3 *to, RayCastHit *output, unsigned int flags, void *null);
    };
    class GameEntityManager
    {
        IMPLMEMBER(EntityManager*, EntityMgr, 0x4);
    };
    class ClientPlayerManager
    {
        IMPLMEMBER(eastl::vector<ClientPlayer*>, Players, 0x94);
        IMPLMEMBER_REL(eastl::vector<ClientPlayer*>, AiPlayers, 0, Players);
        IMPLMEMBER_REL(ClientPlayer*, LocalPlayer, 0, AiPlayers);
    };

    class ClientGameContext
    {
        IMPLMEMBER(GameEntityManager*, EntityMgr, 0x28);
        IMPLMEMBER_REL(ClientPlayerManager*, PlayerMgr, 0, EntityMgr);
    };
}

#endif
