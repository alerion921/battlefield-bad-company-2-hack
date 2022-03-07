#include "Aimbot.h"

#include "main.h"
Aimbot::Aimbot()
{
	//m_controller.tuneManual(0.2f, 0.8f, 0.002f);
	m_controller.tuneManual(0.4f, 0, 0.001f);
}

D3DXVECTOR3 Aimbot::getNextAimAngles(float dt, const D3DXVECTOR3 &camPos, const D3DXVECTOR3 &aimPos, const D3DXVECTOR3 &viewVec)
{
    auto setpoint = GetAbsoluteYawPitch(camPos, aimPos);

    float isYaw = atan2(viewVec.y, viewVec.x);
    float isPitch = atan2(viewVec.z, sqrt(viewVec.x*viewVec.x + viewVec.y*viewVec.y));
    D3DXVECTOR3 measured(isYaw, isPitch, 0);

    //LOG_DBG("setpt: %f / %f  || is: %f / %f\n", setpoint.x, setpoint.y, isYaw, isPitch);

    auto controlled = m_controller.step(dt, setpoint, measured);

    //LOG_DBG("controlled: %f / %f\n", controlled.x, controlled.y);

    return measured + controlled;
}

D3DXVECTOR3 Aimbot::PredictAimPos(const D3DXVECTOR3 &camPos, const D3DXVECTOR3 &targetPos, const D3DXVECTOR3 &targetSpeed, float bulletSpeed, float gravity)
{
    D3DXVECTOR3 predictedAimPos = targetPos;

    // iterate a couple of times to get more accurate bulletTime and predictedAimPos
    for (int i = 0; i < 10; i++) {
        D3DXVECTOR3 bulletPath = predictedAimPos - camPos;
        float bulletTime = D3DXVec3Length(&bulletPath) / bulletSpeed;
        predictedAimPos = targetPos + targetSpeed*bulletTime;
    }

    D3DXVECTOR3 aimPath = D3DXVECTOR3(predictedAimPos.x, predictedAimPos.y, 0) - D3DXVECTOR3(camPos.x, camPos.y, 0);
    float groundDist = D3DXVec3Length(&aimPath);
    float hDist = predictedAimPos.z - camPos.z;

    // ballistic magic~
    float aimHei = (bulletSpeed*bulletSpeed - sqrt((bulletSpeed*bulletSpeed*bulletSpeed*bulletSpeed) -
        (2*bulletSpeed*bulletSpeed*gravity*hDist) - (gravity*gravity*groundDist*groundDist))) / gravity;

    return D3DXVECTOR3(predictedAimPos.x, predictedAimPos.y, predictedAimPos.z + aimHei - hDist);
}


D3DXVECTOR3 Aimbot::GetRelativeYawPitch(const D3DXVECTOR3 &camPos, const D3DXVECTOR3 &aimPos, const D3DXVECTOR3 &viewVec)
{
    D3DXVECTOR3 absolute = GetAbsoluteYawPitch(camPos, aimPos);

    float yaw = atan2(viewVec.y, viewVec.x);
    float pitch = atan2(viewVec.z, sqrt(viewVec.x*viewVec.x + viewVec.y*viewVec.y));

    return D3DXVECTOR3(absolute.x-yaw, absolute.y-pitch, 0);
}

D3DXVECTOR3 Aimbot::GetAbsoluteYawPitch(const D3DXVECTOR3 &camPos, const D3DXVECTOR3 &aimPos)
{
    D3DXVECTOR3 aimVec = aimPos - camPos;
    D3DXVec3Normalize(&aimVec, &aimVec);
    float yaw = atan2(aimVec.y, aimVec.x);
    float pitch = atan2(aimVec.z, sqrt(aimVec.x*aimVec.x + aimVec.y*aimVec.y));

    return D3DXVECTOR3(yaw, pitch, 0);
}