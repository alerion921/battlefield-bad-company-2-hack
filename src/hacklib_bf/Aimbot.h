#ifndef AIMBOT_H
#define AIMBOT_H

#include "d3dx9.h"
#include "PIDController.h"


class Aimbot
{
public:
    Aimbot();

    D3DXVECTOR3 getNextAimAngles(float dt, const D3DXVECTOR3 &camPos, const D3DXVECTOR3 &aimPos, const D3DXVECTOR3 &viewVec);

    static D3DXVECTOR3 PredictAimPos(const D3DXVECTOR3 &camPos, const D3DXVECTOR3 &targetPos, const D3DXVECTOR3 &targetSpeed, float bulletSpeed, float gravity);

    static D3DXVECTOR3 GetRelativeYawPitch(const D3DXVECTOR3 &camPos, const D3DXVECTOR3 &aimPos, const D3DXVECTOR3 &viewVec);
    static D3DXVECTOR3 GetAbsoluteYawPitch(const D3DXVECTOR3 &camPos, const D3DXVECTOR3 &aimPos);

private:
    PIDController<D3DXVECTOR3, float, float> m_controller;

};

#endif
