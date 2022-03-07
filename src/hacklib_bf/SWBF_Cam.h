#ifndef SWBF_CAM_H
#define SWBF_CAM_H

#include "../hacklib/include/hacklib/ImplementMember.h"
#include "d3dx9.h"


namespace SWBF
{
    class Cam
    {
        IMPLMEMBER(D3DXVECTOR3, ViewVec, 0x60);
        IMPLMEMBER(D3DXVECTOR3, Pos, 0x70);
        IMPLMEMBER(float, Fovy, 0xe4);
        IMPLMEMBER(D3DXMATRIX, ViewMat, 0x2a0);
    };

    class CamHolder
    {
        IMPLMEMBER(Cam*, CamData, 0x58);
    };
}

#endif