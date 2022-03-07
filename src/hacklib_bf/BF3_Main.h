#ifndef BF3_MAIN_H
#define BF3_MAIN_H

#include "../hacklib/include/hacklib/ImplementMember.h"
#include "BF3_Library.h"
#include "BF3_Cam.h"
#include "BF3_Input.h"
#include "BF3_Skeleton.h"
#include "BF3_Weapon.h"


namespace BF3
{
    class ClientSoldierReplication
    {
        IMPLMEMBER(fb::Vec3, Velocity, 0x50);
    };

    class VehicleHealthZoneData
    {
        IMPLMEMBER(float, MaxHealth, 0);
    };
    struct HealthZone
    {
        VehicleHealthZoneData *data;
        float health;
        float shieldHealth;
        unsigned int effectdata;
    };
    class ClientVehicleEntityHealth
    {
        IMPLMEMBER(eastl::vector<HealthZone>, HealthZones, 0x14);
    };

    class ClientControllableEntity
    {
        IMPLMEMBER(float, Health, 0x20);
        IMPLMEMBER(ClientAntAnimatableComponent*, AnimatableComponent1, 0xbc);
        IMPLMEMBER_REL(ClientAntAnimatableComponent*, AnimatableComponent2, 0, AnimatableComponent1);
    };
    class ClientVehicleEntity : public ClientControllableEntity
    {
        IMPLMEMBER(fb::Vec3, Speed, 0x140);
        IMPLMEMBER(ClientVehicleEntityHealth*, Health, 0x200);
    };
    class ClientSoldierEntity : public ClientControllableEntity
    {
        IMPLMEMBER(ClientSoldierReplication*, ReplicatedController, 0x24c);
        IMPLMEMBER(fb::LinearTransform, MeshTransform, 0x2c0);
        IMPLMEMBER(ClientBoneCollisionComponent*, BoneCollisionComponent, 0x354);
    };

    class ClientPlayer
    {
        IMPLMEMBER(char*, Name, 0x10);
        IMPLMEMBER(int, TeamId, 0x31c);
        IMPLMEMBER(ClientControllableEntity*, AttachedControllable, 0x3d0);
        IMPLMEMBER_REL(ClientSoldierEntity*, Soldier, 0x4, AttachedControllable);
    };

    class ClientPlayerManager
    {
        IMPLMEMBER(eastl::vector<ClientPlayer*>, Players, 0x9c);
        IMPLMEMBER_REL(ClientPlayer*, LocalPlayer, 0x10, Players);
    };

    class ClientGameContext
    {
        IMPLMEMBER(ClientPlayerManager*, PlayerMgr, 0x30);
    };
}

#endif