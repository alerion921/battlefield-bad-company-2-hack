#ifndef BF4_MAIN_H
#define BF4_MAIN_H

#include "../hacklib/include/hacklib/ImplementMember.h"
#include "BF4_Library.h"
#include "BF4_Input.h"
#include "BF4_Cam.h"


namespace BF4
{
    class ClientSoldierReplication
    {
        IMPLMEMBER(fb::Vec3, Position, 0x30);
        IMPLMEMBER(fb::Vec3, Velocity, 0x50);
    };

    class SoldierHealthData
    {
        IMPLMEMBER(float, Health, 0x20);
        IMPLMEMBER_REL(float, MaxHealth, 0, Health);
    };

    class ClientControllableEntity
    {
        IMPLMEMBER(SoldierHealthData*, HealthData, 0x140);
        // 1e0: bones stuff (+538(animatable) / +58(updateposeresultdata.activeworldtrans) || +128 / +50)
        //IMPLMEMBER(ClientAntAnimatableComponent*, AnimatableComponent1, 0xbc);
        //IMPLMEMBER_REL(ClientAntAnimatableComponent*, AnimatableComponent2, 0, AnimatableComponent1);
    };
    class ClientVehicleEntity : public ClientControllableEntity
    {
        IMPLMEMBER(fb::Vec3, Velocity, 0x280);
    };
    class ClientSoldierEntity : public ClientControllableEntity
    {
        // 480: physicsentity
        IMPLMEMBER(ClientSoldierReplication*, ReplicatedController, 0x490);
        // 550: weapon component
        IMPLMEMBER(bool, IsOccluded, 0x591);
        IMPLMEMBER(fb::LinearTransform, MeshTransform, 0x1800);
        //IMPLMEMBER(ClientBoneCollisionComponent*, BoneCollisionComponent, 0x354);
    };

    class ClientPlayer
    {
        IMPLMEMBER(char*, Name, 0x18);
        IMPLMEMBER(int, TeamId, 0x13cc);
        IMPLMEMBER(eastl::WeakPtr<ClientSoldierEntity>, Character, 0x14b0);
        IMPLMEMBER(ClientControllableEntity*, AttachedControllable, 0x14c0);
        IMPLMEMBER(ClientControllableEntity*, ControlledControllable, 0x14d0);
    };

    class ClientPlayerManager
    {
        IMPLMEMBER(eastl::vector<ClientPlayer*>, Players, 0xd0);
        //IMPLMEMBER(ClientPlayer*, LocalPlayer, 0x2a0);
        //IMPLMEMBER(eastl::vector<ClientPlayer*>, Players, 0x370);
        IMPLMEMBER(ClientPlayer*, LocalPlayer, 0x540);
    };

    class ClientGameWorld
    {
        IMPLMEMBER(void*, TypeOffset, 0x28);
    };

    class ClientLevel
    {
        IMPLMEMBER(ClientGameWorld*, GameWorld, 0x130);
    };

    class ClientGameContext
    {
        IMPLMEMBER(ClientPlayerManager*, PlayerMgr, 0x18);
        IMPLMEMBER(ClientLevel*, Level, 0x28);
    };
}

#endif