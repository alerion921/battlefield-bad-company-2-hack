#ifndef BFBC2_CAM_H
#define BFBC2_CAM_H

#include "../hacklib/include/hacklib/ImplementMember.h"
#include "d3dx9.h"


namespace BFBC2
{
    class Cam
    {
        IMPLMEMBER(D3DXVECTOR3, Pos, 0x30);
        IMPLMEMBER(D3DXVECTOR3, ViewVec, 0xf0);
    };
}

#endif
