#ifndef BF3_CAM_H
#define BF3_CAM_H

#include "../hacklib/include/hacklib/ImplementMember.h"
#include "d3dx9.h"


namespace BF3
{
    class Cam
    {
        IMPLMEMBER(D3DXVECTOR3, ViewVec, 0xc0);
        IMPLMEMBER(D3DXVECTOR3, Pos, 0xd0);
        IMPLMEMBER(float, Fovy, 0xa18);
    };
}

#endif