#ifndef BF4_CAM_H
#define BF4_CAM_H

#include "../hacklib/include/hacklib/ImplementMember.h"
#include "d3dx9.h"


namespace BF4
{
    class Cam
    {
        IMPLMEMBER(D3DXMATRIX, Transform, 0x40);
        IMPLMEMBER(D3DXVECTOR3, ViewVec, 0x60);
        IMPLMEMBER(D3DXVECTOR3, Pos, 0x70);
        IMPLMEMBER(float, Fovy, 0xb4);
        IMPLMEMBER(D3DXMATRIX, ViewMat, 0x260);
    };

    class CamHolder
    {
        IMPLMEMBER(Cam*, CamData, 0x60);
    };
}

#endif