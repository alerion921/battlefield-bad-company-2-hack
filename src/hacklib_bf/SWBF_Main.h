#ifndef SWBF_MAIN_H
#define SWBF_MAIN_H

#include "../hacklib/include/hacklib/ImplementMember.h"
#include "SWBF_Library.h"
#include "SWBF_Input.h"
#include "SWBF_Cam.h"


namespace SWBF
{
    class ClientSoldierReplication
    {
        IMPLMEMBER(fb::Vec3, Position, 0x20);
        IMPLMEMBER(fb::Vec3, Velocity, 0x40);
    };

    class SoldierHealthData
    {
        IMPLMEMBER(float, Health, 0x20);
        IMPLMEMBER_REL(float, MaxHealth, 0, Health);
    };

    class ClientControllableEntity
    {
        IMPLMEMBER(SoldierHealthData*, HealthData, 0x198);
    };
    class ClientVehicleEntity : public ClientControllableEntity
    {
        IMPLMEMBER(fb::Vec3, Velocity, 0x280);
    };
    class ClientSoldierEntity : public ClientControllableEntity
    {
        IMPLMEMBER(ClientSoldierReplication*, ReplicatedController, 0x4f8);
        IMPLMEMBER(fb::LinearTransform, MeshTransform, 0x1490);
    };

    class ClientPlayer
    {
        IMPLMEMBER(char*, Name, 0x18);
        IMPLMEMBER(int, TeamId, 0x13bc);
        IMPLMEMBER(eastl::WeakPtr<ClientSoldierEntity>, Character, 0x14a8);
        IMPLMEMBER(ClientControllableEntity*, AttachedControllable, 0x14b8);
        IMPLMEMBER(ClientControllableEntity*, ControlledControllable, 0x14c8);
    };

    class ClientPlayerManager
    {
        IMPLMEMBER(eastl::vector<ClientPlayer*>, Players, 0xd8);
        IMPLMEMBER(ClientPlayer*, LocalPlayer, 0x550);
    };

    class ClientGameContext
    {
        IMPLMEMBER(ClientPlayerManager*, PlayerMgr, 0x20);
    };
}

#endif