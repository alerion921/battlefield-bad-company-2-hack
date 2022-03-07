#ifndef BF4_WEAPON_H
#define BF4_WEAPON_H

#include "ImplementMember.h"
#include "d3dx9.h"


namespace BF4
{
    class ShotConfigData
    {
        IMPLMEMBER(D3DXVECTOR3, InitialVelocity, 0x20);
    };

    class FiringFunctionData
    {
        IMPLMEMBER(ShotConfigData, Shot, 0x60);
    };

    class WeaponFiringData
    {
        IMPLMEMBER(FiringFunctionData*, PrimaryFire, 0x10);
    };

    class SoldierWeaponData
    {
        IMPLMEMBER(WeaponFiringData*, WeaponFiring, 0x18);
    };

    class ClientSoldierWeapon
    {
        IMPLMEMBER(SoldierWeaponData*, Data, 0x49a8);
    };
}

#endif