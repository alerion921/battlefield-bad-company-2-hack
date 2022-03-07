#include "Hack.h"
#include "main.h"
#include "glm/gtc/matrix_transform.hpp"
#include <thread>


static void cbConIn(std::string input)
{
    LOG_INFO("> %s\n", input.c_str());

    if (input == "exit")
        GetMain()->getHack()->stop();
    else if (input == "distance")
        GetMain()->getHack()->toggleDistance();
    else if (input == "names")
        GetMain()->getHack()->toggleNames();
    else
        LOG_INFO("undefined commnad\n");
}


Hack::Hack() : m_con(hl::GetCurrentModule()), m_overlay(hl::GetCurrentModule())
{
    m_running = false;
    m_showDistance = false;
    m_showNames = false;

    m_target = nullptr;
}


hl::ConsoleEx *Hack::getCon()
{
    return &m_con;
}


bool Hack::runStep()
{
    if (!m_running || !m_con.isOpen())
        return false;

    std::this_thread::sleep_for(std::chrono::milliseconds(13));

    return true;
}
void Hack::run()
{
    if (m_running)
        return;

    if (!init()) {
        LOG_ERR("failed to initalize hack\n");
        while (m_con.isOpen())
            std::this_thread::sleep_for(std::chrono::milliseconds(25));
        return;
    }

    LOG_INFO("# RUNNING\n");
    m_running = true;

    while (m_running && m_con.isOpen())
    {
        __try {
            update();
            gameCallback();
        } __except (EXCEPTION_EXECUTE_HANDLER) {
            LOG_ERR("exception in external game update thread\n");
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(13));
    }

    LOG_INFO("# STOPPED\n");
//    safeUnhook();

  //  m_drawer.ReleaseRessources();
   m_overlay.close();
   m_con.close();

   m_running = false;
}

void Hack::stop()
{
    m_running = false;
}

void Hack::toggleDistance()
{
    m_showDistance = !m_showDistance;
}

void Hack::toggleNames()
{
    m_showNames = !m_showNames;
}


void Hack::gameCallback()
{
    m_input.update();

    auto dev = m_overlay.getContext();
    if (!dev)
        return;

    m_overlay.beginDraw();

#ifdef _DEBUG
    m_drawer.drawRect(0, 0, m_overlay.getWidth()-1.0f, m_overlay.getHeight()-1.0f, 0xffff3300);
#endif
    processGameData();

    m_overlay.swapBuffers();
}



bool Hack::init()
{
    std::string conTitle = "HAEKX ";
    conTitle += VERSION_S;

    m_con.registerInputCallback(cbConIn);
    hl::CONSOLEEX_PARAMETERS params = hl::ConsoleEx::GetDefaultParameters();
    params.cellsYBuffer = 5000;
    params.closemenu = true;
    if (!m_con.create(conTitle, &params))
    {
        LOG_ERR("failed to create console\n");
        return false;
    }

    LOG_INFO("###########################################\n"
             "###  HAEKX EXTERNAL OVERLAY  %s"     "  ###\n"
             "###########################################\n", VERSION_S);
    LOG_INFO("# LOADING...\n");

    LOG_DBG("scanning target process\n");
    if (!BattlefieldDataMgr::init()) {
        LOG_ERR("error while scanning target process\n");
        return false;
    }

    LOG_DBG("creating overlay\n");
    switch (m_overlay.create())
    {
    case hl::GfxOverlay::Error::Window:
        LOG_ERR("failed to create overlay window\n");
        return false;
    case hl::GfxOverlay::Error::Context:
        LOG_ERR("failed to create overlay graphics device\n");
        return false;
    case hl::GfxOverlay::Error::Other:
        LOG_ERR("failed to create the overlay\n");
        return false;
    }

    // Target an impossibly high frame rate, so that swapBuffers does not sleep.
    m_overlay.setTargetRefreshRate(10000);
    m_overlay.registerResetHandlers([this]{ m_drawer.onLostDevice(); }, [this]{ m_drawer.onResetDevice(); });

    m_drawer.SetDevice(m_overlay.getContext());

    m_running = true;

    LOG_INFO("# DONE!\n");
    return true;
}

void Hack::processGameData()
{
    auto data = getData();
    if (!data || !data->isIngame())
        return;

#ifdef _DEBUG
    m_drawer.DrawFont(50, 50, 0xffffffff, "pCtx: %p", data->getInternalRep());
    if (data->getOwnData())
        m_drawer.DrawFont(50, 80, 0xffffffff, "pOwnData: %p", data->getOwnData()->getInternalRep());
    if (data->getWeapon())
        m_drawer.DrawFont(50, 95, 0xffffffff, "pWeapon: %p", data->getWeapon()->getInternalRep());
#endif

    auto camData = data->getCamData();
    if (!camData)
        return;

#ifdef _DEBUG
    m_drawer.DrawFont(50, 65, 0xffffffff, "pCamData: %p", camData->getInternalRep());
#endif

    auto camPos = camData->getPos();
    auto viewVec = camData->getViewVec();
    auto lookAt = camPos + viewVec;

    // set the view and projection parameters
    hl::Mat4x4 viewMat = camData->getViewMat();
    auto projMat = glm::perspectiveFovRH(camData->getFovY(), (float)m_overlay.getWidth(), (float)m_overlay.getHeight(), 0.1f, 100000.0f);
    m_drawer.update(viewMat, projMat);

    auto testPos = D3DXVECTOR3(0, 0, 0);
    D3DXVECTOR3 targetViewspace;
	//D3DXVec3TransformCoord(&targetViewspace, &testPos, &viewMat);
	//D3DXVec3TransformCoord(&targetViewspace, &targetViewspace, &projMat);
    D3DXVECTOR3 targetScreen;
    //m_drawer.Project(targetViewspace, targetScreen);
    //m_drawer.DrawCircleFilled(targetViewspace.x, targetViewspace.y, 10, 0xccff0000);
	//m_drawer.DrawFont(50, 300, 0xff000000, "viewspace: %f %f %f\n", targetViewspace.x, targetViewspace.y, targetViewspace.z);
	//float mx = (1.0f + targetViewspace.x) * m_drawer.GetWidth() / 2.0f;
	//float my = (1.0f - targetViewspace.y) * m_drawer.GetHeight() / 2.0f;
    //m_drawer.DrawCircleFilled(mx, my, 10, 0xccff0000);
    float len1 = sqrt(targetViewspace.x*targetViewspace.x + targetViewspace.z*targetViewspace.z);
    float len2 = sqrt(viewVec.x*viewVec.x + viewVec.y*viewVec.y);
    float angle = acos((targetViewspace.x*viewVec.x + -targetViewspace.z*viewVec.y) /
        (len1 * len2));
    //float angle = atan2(-targetViewspace.x, -targetViewspace.z);
	//m_drawer.DrawFont(50, 330, 0xff000000, "angle: %f\n", angle);

    m_drawer.DrawFont(50, 500, 0xff000000, "numplayers: %i\n", data->getPlayers().size());
	//m_drawer.DrawFont(50, 550, 0xff000000, "thing:: %p\n", data->getPlayers()[data->getPlayers().size() - 1].getInternalRep());

    auto ownPlayer = data->getOwnPlayer();
    if (!ownPlayer)
        return;

#ifdef _DEBUG
    m_drawer.drawLineProjected(D3DXVECTOR3(0,0,0), D3DXVECTOR3(100,0,0), 0xffff3333);
    m_drawer.drawLineProjected(D3DXVECTOR3(0,0,0), D3DXVECTOR3(0,100,0), 0xff33ff33);
    m_drawer.drawLineProjected(D3DXVECTOR3(0,0,0), D3DXVECTOR3(0,0,100), 0xff3333ff);

    float camYaw = atan2(camData->getViewVec().y, camData->getViewVec().x) + D3DX_PI;
    float camPitch = atan2(camData->getViewVec().z, sqrt(camData->getViewVec().x*camData->getViewVec().x + camData->getViewVec().y*camData->getViewVec().y));
    //float camRoll = -(atan2(viewMat._32, viewMat._31) - D3DX_PI/2);
    float camRoll = atan2(viewMat[2][1], viewMat[2][0]);// - D3DX_PI/2);
    if (camYaw > D3DX_PI)
        camYaw = -(2*D3DX_PI - camYaw);
    m_drawer.DrawFont(50, 200, 0xff000000, "Cam:  yaw: %f pitch: %f roll: %f\n", camYaw, camPitch, camRoll);

    m_drawer.DrawFont(50, 115, 0xffffffff, "pPlayer: %p", ownPlayer->getInternalRep());
    if (ownPlayer->getSkeleton())
        m_drawer.DrawFont(50, 130, 0xffffffff, "pSkeleton: %p", ownPlayer->getSkeleton()->getInternalRep());
#endif

    m_drawer.DrawOwnInfo(ownPlayer, data->getOwnData());

    float dt = m_timer.diff();
    m_timer.reset();

    float closestPhi = std::numeric_limits<float>::max();
    bool targetFound = false;

    for (const auto& p : data->getPlayers()) {
        if (!p.isValid())
            continue;

        if (p.getTeamId() == ownPlayer->getTeamId())
            continue;

        if (m_input.wentDown(VK_CONTROL))
        {
            D3DXVECTOR3 aimPos;
            auto skeleton = p.getSkeleton();
            if (skeleton && skeleton->getHeadBone())
                aimPos = skeleton->getHeadBone()->pos;
            else
                aimPos = p.getPos() + D3DXVECTOR3(0, 0, 1);

            auto aimVec = aimPos - camPos;
            D3DXVec3Normalize(&aimVec, &aimVec);
            float phi = acos(D3DXVec3Dot(&viewVec, &aimVec));
            if (phi < closestPhi) {
                closestPhi = phi;
                m_target = &p;
            }
        }

        if (m_target == &p)
            targetFound = true;

        m_drawer.DrawPlayer(&p, camData, p.getName() == "ToBeDe" ? 0xccffff00 : 0xccff3300, m_showDistance, m_showNames);
    }

    if (m_target && (!targetFound || m_input.wentUp(VK_CONTROL)))
        m_target = nullptr;

    // test
    if (m_input.isDown(VK_NUMPAD1))
    {
        setAimAngles(0, 0);
        return;
    }
    if (m_input.isDown(VK_NUMPAD2))
    {
        setAimAngles(0.5f, 0.5f);
        return;
    }
    if (m_input.isDown(VK_NUMPAD3))
    {
        setAimAngles(D3DX_PI/2, (D3DX_PI/2)-1.0f);
        return;
    }

    auto weapon = data->getWeapon();
    if (m_target && weapon && m_input.isDown(VK_CONTROL))
    {
        D3DXVECTOR3 aimPos;
        auto skeleton = m_target->getSkeleton();
        if (skeleton && skeleton->getHeadBone())
            aimPos = skeleton->getHeadBone()->pos;
        else
            aimPos = m_target->getPos() + D3DXVECTOR3(0, 0, 1);

        auto predictedAimPos = Aimbot::PredictAimPos(camPos, aimPos, m_target->getVel(), data->getWeapon()->getBulletSpeed(), data->getWeapon()->getGravity());
		auto yawPitch = m_aimbot.getNextAimAngles(dt, camPos, predictedAimPos, viewVec);
		//auto yawPitch = Aimbot::GetAbsoluteYawPitch(camPos, predictedAimPos);
        //LOG_INFO("yaw: %f , pitch: %f\n", yawPitch.x, yawPitch.y);

        auto dist = aimPos - camPos;
		//auto targetPos = aimPos + m_target->getVel() * (2.5f / sqrt(D3DXVec3Length(&dist)));
		//auto targetPos = aimPos + m_target->getVel() * 0.25f;
        auto targetPos = aimPos + m_target->getVel() * 0.20f;
        //auto predictedAimPos = Aimbot::PredictAimPos(camPos, aimPos, m_target->getVel(), weapon->getBulletSpeed(), weapon->getGravity());
        auto distVec = predictedAimPos - camPos;
		//auto targetPos = aimPos + m_target->getVel() * (4.5f / sqrt(D3DXVec3Length(&distVec)));

        /*D3DXVECTOR3 screen;
        m_drawer.Project(predictedAimPos, screen);
        m_drawer.DrawCircle(screen.x, screen.y, 10.0f, 0xccff0000);*/

        setAimPos(targetPos);
        setAimAngles(yawPitch.x, yawPitch.y);
    }
}
