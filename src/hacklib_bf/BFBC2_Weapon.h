#ifndef BFBC2_WEAPON_H
#define BFBC2_WEAPON_H

#include "../hacklib/include/hacklib/ImplementMember.h"
#include "BFBC2_Library.h"


namespace BFBC2
{
    struct ShotConfigData
    {
        fb::Vec3 initialSpeed;
    };

    class FiringFunctionData
    {
        IMPLMEMBER(ShotConfigData, Shot, 0xa0);
    };

    class WeaponFiringData
    {
        IMPLMEMBER(FiringFunctionData*, PrimaryFire, 0x40);
    };

    class SoldierWeaponData
    {
        IMPLMEMBER(char*, Name, 0xc);
        IMPLMEMBER(WeaponFiringData*, WeaponFiring, 0x98);
    };

    class ClientSoldierWeapon
    {
        IMPLMEMBER(SoldierWeaponData*, Data, 0x4);
    };
}

#endif
