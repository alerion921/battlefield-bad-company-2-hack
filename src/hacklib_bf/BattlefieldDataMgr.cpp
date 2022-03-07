#include "BattlefieldDataMgr.h"
#include "main.h"
#include "../hacklib/include/hacklib/PatternScanner.h"

using std::uintptr_t;


static BattlefieldDataMgr *g_this = nullptr;
static const char moduleNameBFBC2[] = "BFBC2Game.exe";
static const char moduleNameBF3[] = "bf3.exe";
static const char moduleNameBF4[] = "bf4.exe";
static const char moduleNameSWBF[] = "starwarsbattlefront.exe";


void __fastcall hkBFBC2InputUpdate(BFBC2::BorderInputNode *pInst, int, float deltaTime);
void __fastcall hkBF3InputUpdate(BF3::BorderInputNode *pInst, int, float deltaTime);
void __fastcall hkBF4InputUpdate(BF4::IMouse *pInst, int, float deltaTime);
void __fastcall hkSWBFInputUpdate(SWBF::IMouse *pInst, int, float deltaTime);


BattlefieldDataMgr::BattlefieldDataMgr()
{
    g_this = this;

    m_hkBFBC2InputUpdate = nullptr;
    m_hkBF3InputUpdate = nullptr;
    m_hkBF4InputUpdate = nullptr;

    m_overwriteInput = false;
    m_inputYaw = 0;
    m_inputPitch = 0;
    m_camYawDiff = 0;
    m_camPitchDiff = 0;

    m_version = Version::Unknown;
}

BattlefieldDataMgr *BattlefieldDataMgr::getInstance()
{
    return g_this;
}


bool BattlefieldDataMgr::init()
{
    // get module base
    HMODULE hMod = GetModuleHandle(moduleNameBFBC2);
    if (hMod) {
        m_version = Version::BFBC2;
    } else {
        hMod = GetModuleHandle(moduleNameBF3);
        if (hMod) {
            m_version = Version::BF3;
        } else {
            hMod = GetModuleHandle(moduleNameBF4);
            if (hMod) {
                m_version = Version::BF4;
            } else {
                hMod = GetModuleHandle(moduleNameSWBF);
                if (hMod) {
                    m_version = Version::SWBF;
                } else {
                    LOG_ERR("could not find any battlefield module\n");
                    return false;
                }
            }
        }
    }

    switch(m_version)
    {
    case Version::BFBC2:
        LOG_INFO("# MODE: BFBC2\n");
        return InitBFBC2();
    case Version::BF3:
        LOG_INFO("# MODE: BF3\n");
        return InitBF3();
    case Version::BF4:
        LOG_INFO("# MODE: BF4\n");
        return InitBF4();
    case Version::SWBF:
        LOG_INFO("# MODE: SWBF\n");
        return InitSWBF();
    }
    return false;
}

void BattlefieldDataMgr::update()
{
    m_data.m_internalRep = nullptr;
    __try {
        switch (m_version)
        {
        case Version::BFBC2:
            UpdateBFBC2();
            break;
        case Version::BF3:
            UpdateBF3();
            break;
        case Version::BF4:
            UpdateBF4();
            break;
        case Version::SWBF:
            UpdateSWBF();
            break;
        }
    } __except (EXCEPTION_EXECUTE_HANDLER) {
        LOG_DBG("exception on update\n");
        m_data.m_internalRep = nullptr;
    }
}


const BattlefieldData *BattlefieldDataMgr::getData() const
{
    if (m_data.m_internalRep)
        return &m_data;
    return nullptr;
}


void BattlefieldDataMgr::setAimAngles(float yaw, float pitch)
{
    m_inputYaw = yaw;
    m_inputPitch = pitch;
    m_overwriteInput = true;
}

void BattlefieldDataMgr::setAimPos(const D3DXVECTOR3& pos)
{
    m_targetPos = pos;
}

bool BattlefieldDataMgr::hkInputUpdateCam(float *yaw, float *pitch)
{
    update();

    __try {
        gameCallback();
    } __except (EXCEPTION_EXECUTE_HANDLER) {
        LOG_ERR("unexpected error in game thread\n");
    }

    if (m_overwriteInput) {
        auto cam = m_data.getCamData();
        if (cam)
        {
            float camYaw = atan2(cam->getViewVec().y, cam->getViewVec().x);
            float camPitch = atan2(cam->getViewVec().z, sqrt(cam->getViewVec().x*cam->getViewVec().x + cam->getViewVec().y*cam->getViewVec().y));
            //LOG_DBG("current cam angles: %f %f\n", camYaw, camPitch);
            //LOG_DBG("should be angles: %f %f\n", m_inputYaw, m_inputPitch);

            //*yaw = 0.039f * (m_inputYaw - camYaw);
            //*pitch = 0.039f * (m_inputPitch - camPitch);
            *yaw = (1 / cam->getFovY()) * 0.024f * (m_inputYaw - camYaw);
            *pitch = (1 / cam->getFovY()) *0.024f * (m_inputPitch - camPitch);
            //LOG_DBG("diff angles: %f %f\n", *yaw, *pitch);
            m_overwriteInput = false;
            return true;
        }
    }
    return false;
}


bool BattlefieldDataMgr::InitBFBC2()
{
    uintptr_t sigIngame = hl::FindPattern("83 c4 04 8b ce e8 ?? ?? ?? ?? 5b");
    uintptr_t sigCtx = hl::FindPattern("b8 01 00 00 00 84 05 ?? ?? ?? ?? 75 66");
    uintptr_t sigCam = hl::FindPattern("8d 47 10 81 c3");
    uintptr_t sigFovy = hl::FindPattern("39 5e 38 74 6f");
    uintptr_t sigMystate = hl::FindPattern("8b 48 18 8b 11 8d 46 2c");
    uintptr_t sigInput = hl::FindPattern("8b 0d ?? ?? ?? ?? 8b 01 8b 50 0c ff d2 8b 0d");

    LOG_DBG("sigingame      %p\n", sigIngame);
    LOG_DBG("sigctx         %p\n", sigCtx);
    LOG_DBG("sigcam         %p\n", sigCam);
    LOG_DBG("sigfovy        %p\n", sigFovy);
    LOG_DBG("sigmystate     %p\n", sigMystate);
    LOG_DBG("siginput       %p\n", sigInput);

    if (!sigIngame || !sigCtx ||!sigCam || !sigFovy || !sigMystate || !sigInput) {
        LOG_DBG("one or more patterns were not found\n");
        return false;
    }

    if (![&]{
        __try {
            m_memsBFBC2.pIsIngame = *(unsigned char**)(hl::FollowRelativeAddress(sigIngame - 4) + 5);
            m_memsBFBC2.pCtx = *(BFBC2::ClientGameContext**)(sigCtx + 0x15);
            m_memsBFBC2.pCam = (const BFBC2::Cam**)(*(uintptr_t*)(sigCam + 0x5) + 0x30);
            m_memsBFBC2.fovy = *(uintptr_t*)(sigFovy + 0x7);
            m_memsBFBC2.pMystate = *(const BFBC2::Mystate***)(sigMystate - 0x9);
            m_memsBFBC2.pInput = *(const BFBC2::BorderInputNode***)(sigInput + 0x2);
            return true;
        }
        __except (EXCEPTION_EXECUTE_HANDLER) {
            LOG_DBG("one or more patterns are invalid\n");
            return false;
        }
    }())
    {
        return false;
    }

    if (!m_memsBFBC2.pIsIngame || !m_memsBFBC2.pCtx || !m_memsBFBC2.pCam || !m_memsBFBC2.fovy || !m_memsBFBC2.pMystate || !m_memsBFBC2.pInput) {
        LOG_DBG("one or more patterns resulted in invalid mems\n");
        return false;
    }

    LOG_DBG("---------------\n");
    LOG_DBG("memisingame    %p\n", m_memsBFBC2.pIsIngame);
    LOG_DBG("memctx         %p\n", m_memsBFBC2.pCtx);
    LOG_DBG("memcam         %p\n", m_memsBFBC2.pCam);
    LOG_DBG("memfovy        %p\n", m_memsBFBC2.fovy);
    LOG_DBG("memmystate     %p\n", m_memsBFBC2.pMystate);
    LOG_DBG("meminput       %p\n", m_memsBFBC2.pInput);

    m_hkBFBC2InputUpdate = m_hooker.hookVT(*m_memsBFBC2.pInput, 20, hkBFBC2InputUpdate);
    if (!m_hkBFBC2InputUpdate) {
        LOG_DBG("hooking input update failed\n");
        return false;
    }

    return true;
}

bool BattlefieldDataMgr::InitBF3()
{
    uintptr_t sigCtx = hl::FindPattern("83 c7 04 3b fb 75 f0 8b 0d");
    uintptr_t sigCam = hl::FindPattern("51 8b 0d ?? ?? ?? ?? d9 1c 24 83 c0 10");
    uintptr_t sigInput = hl::FindPattern("8b 35 ?? ?? ?? ?? 85 f6 74 5c");

    LOG_DBG("sigctx         %p\n", sigCtx);
    LOG_DBG("sigcam         %p\n", sigCam);
    LOG_DBG("siginput       %p\n", sigInput);

    if (!sigCtx || !sigCam || !sigInput) {
        LOG_DBG("one or more pattern were not found\n");
        return false;
    }

    if (![&]{
        __try {
            m_memsBF3.pCtx = **(BF3::ClientGameContext***)(sigCtx + 0x9);
            m_memsBF3.pCam = **(BF3::Cam***)(sigCam + 0x3);
            m_memsBF3.pInput = *(BF3::BorderInputNode**)(sigInput + 0x2);
            return true;
        }
        __except (EXCEPTION_EXECUTE_HANDLER) {
            LOG_DBG("one or more patterns are invalid\n");
            return false;
        }
    }())
    {
        return false;
    }

    if (!m_memsBF3.pCtx || !m_memsBF3.pCam || !m_memsBF3.pInput) {
        LOG_DBG("one or more patterns resulted in invalid mems\n");
        return false;
    }

    LOG_DBG("---------------\n");
    LOG_DBG("memctx         %p\n", m_memsBF3.pCtx);
    LOG_DBG("memcam         %p\n", m_memsBF3.pCam);
    LOG_DBG("meminput       %p\n", m_memsBF3.pInput);

    // TODO hook input

    return true;
}

bool BattlefieldDataMgr::InitBF4()
{
    uintptr_t sigCtx = hl::FindPattern("45 84 f6 75 ?? 84 db 75 ?? 48 8b 05 ?? ?? ?? ?? 48 8b 48");
    uintptr_t sigCam = hl::FindPattern("84 c0 75 ?? 48 8b 0d ?? ?? ?? ?? 48 8b 01 ff 50 ?? f3 0f 10 0d");
    uintptr_t sigInput = hl::FindPattern("0f 28 c8 0f 29 74 24 ?? 0f 28 f0");

    LOG_DBG("sigctx         %p\n", sigCtx);
    LOG_DBG("sigcam         %p\n", sigCam);
    LOG_DBG("siginput       %p\n", sigInput);

    if (!sigCtx || !sigCam  || !sigInput) {
        LOG_DBG("one or more pattern were not found\n");
        return false;
    }

    if (![&]{
        __try {
            m_memsBF4.pCtx = *(BF4::ClientGameContext**)hl::FollowRelativeAddress(sigCtx + 0xc);
            LOG_DBG("ctx: %p\n", m_memsBF4.pCtx);

            m_memsBF4.pCamHolder = *(BF4::CamHolder**)hl::FollowRelativeAddress(sigCam + 0x7);
            LOG_DBG("cam: %p\n", m_memsBF4.pCamHolder);

            m_memsBF4.pInput = *(BF4::BorderInputNode**)hl::FollowRelativeAddress(sigInput - 0x4);
            LOG_DBG("input: %p\n", m_memsBF4.pInput);

            return true;
        }
        __except (EXCEPTION_EXECUTE_HANDLER) {
            LOG_DBG("one or more patterns are invalid\n");
            return false;
        }
    }())
    {
        return false;
    }

    if (!m_memsBF4.pCtx || !m_memsBF4.pCamHolder || !m_memsBF4.pInput) {
        LOG_DBG("one or more patterns resulted in invalid mems\n");
        return false;
    }

    LOG_DBG("---------------\n");
    LOG_DBG("memctx         %p\n", m_memsBF4.pCtx);
    LOG_DBG("memcam         %p\n", m_memsBF4.pCamHolder);
    LOG_DBG("meminput       %p\n", m_memsBF4.pInput);

    m_hkBF4InputUpdate = m_hooker.hookVT(m_memsBF4.pInput->getMouse(), 3, hkBF4InputUpdate);
    if (!m_hkBF4InputUpdate) {
        LOG_DBG("hooking input update failed\n");
        return false;
    }

    return true;
}

bool BattlefieldDataMgr::InitSWBF()
{
    uintptr_t sigCtx = hl::FindPattern("ff 50 08 48 8b 05 ?? ?? ?? ?? 48 8b 58 28");
    uintptr_t sigCam = hl::FindPattern("41 ff 52 ?? 48 8b 0d ?? ?? ?? ?? 48 8b 01 ff 50 ?? 4c 8b 74 24");
    uintptr_t sigInput = hl::FindPattern("0f 29 74 24 20 48 3b 3d");

    LOG_DBG("sigctx         %p\n", sigCtx);
    LOG_DBG("sigcam         %p\n", sigCam);
    LOG_DBG("siginput       %p\n", sigInput);

    if (!sigCtx || !sigCam || !sigInput) {
        LOG_DBG("one or more pattern were not found\n");
        return false;
    }

    if (![&]{
        __try {
            m_memsSWBF.pCtx = *(SWBF::ClientGameContext**)hl::FollowRelativeAddress(sigCtx + 0x6);
            LOG_DBG("ctx: %p\n", m_memsSWBF.pCtx);

            m_memsSWBF.pCamHolder = *(SWBF::CamHolder**)hl::FollowRelativeAddress(sigCam + 0x7);
            LOG_DBG("cam: %p\n", m_memsSWBF.pCamHolder);

            m_memsSWBF.pInput = *(**(SWBF::BorderInputNode****)hl::FollowRelativeAddress(sigInput - 0x4) + 1);
            LOG_DBG("input: %p\n", m_memsSWBF.pInput);

            return true;
        }
        __except (EXCEPTION_EXECUTE_HANDLER) {
            LOG_DBG("one or more patterns are invalid\n");
            return false;
        }
    }())
    {
        return false;
    }

    if (!m_memsSWBF.pCtx || !m_memsSWBF.pCamHolder || !m_memsSWBF.pInput) {
        LOG_DBG("one or more patterns resulted in invalid mems\n");
        return false;
    }

    LOG_DBG("---------------\n");
    LOG_DBG("memctx         %p\n", m_memsSWBF.pCtx);
    LOG_DBG("memcam         %p\n", m_memsSWBF.pCamHolder);
    LOG_DBG("meminput       %p\n", m_memsSWBF.pInput);

    m_hkSWBFInputUpdate = m_hooker.hookVT(m_memsSWBF.pInput->getMouse(), 3, hkSWBFInputUpdate);
    if (!m_hkSWBFInputUpdate) {
        LOG_DBG("hooking input update failed\n");
        return false;
    }

    return true;
}



void BattlefieldDataMgr::UpdateBFBC2Skeleton(Skeleton *pSkel, const BFBC2::AnimatedSoldier *pData)
{
    pSkel->m_internalRep = nullptr;

    if (pData && 
        pData->getModelBase() && pData->getModelBase()->getWorldPose() &&
        pData->getSkeleton() && pData->getSkeleton()->getSkeleton() && pData->getSkeleton()->getSkeleton()->Bones)
    {
        pSkel->m_internalRep = (void*)pData->getSkeleton();
        pSkel->m_headIndex = -1;
        pSkel->m_chestIndex = -1;
        pSkel->m_hipsIndex = -1;

        D3DXMATRIX *worldTrans = pData->getModelBase()->getWorldPose()->first;
        auto skel = pData->getSkeleton()->getSkeleton();

        auto& bones = pSkel->m_bones;
        bones.resize(skel->BoneCount);
        for (int i = 0; i < skel->BoneCount; i++)
        {
            bool ignore = false;

            const char *name = skel->Bones[i].Name;
            if (strcmp(name, "grannyRootBone") == 0 || strcmp(name, "cameraBase") == 0)
                ignore = true;
            else if (strcmp(name, "Head") == 0)
                pSkel->m_headIndex = i;
            else if (strcmp(name, "Spine2") == 0)
                pSkel->m_chestIndex = i;
            else if (strcmp(name, "Hips") == 0)
                pSkel->m_hipsIndex = i;

            bones[i].ignore = ignore;
            bones[i].parent = skel->Bones[i].PartentIndex;
            bones[i].pos = D3DXVECTOR3(-worldTrans[i]._41, worldTrans[i]._43, worldTrans[i]._42);
        }
    }
}
void BattlefieldDataMgr::UpdateBFBC2Player(Player *pPlayer, const BFBC2::ClientPlayer *pData, bool isOwn)
{
    pPlayer->m_internalRep = nullptr;

    if (pData && pData->getSoldier().offsettedSoldier)
    {
        pPlayer->m_teamId = pData->getTeamId();
        pPlayer->m_name = std::string(pData->getName());

        BFBC2::ClientSoldierEntity *pSoldier = (BFBC2::ClientSoldierEntity*)(*pData->getSoldier().offsettedSoldier - 0x4);
        if (pSoldier)
        {
            pPlayer->m_internalRep = (void*)pData;

            if (pSoldier->getCollection()) {
                auto trans = *(BFBC2::fb::LinearTransform*)((uintptr_t)pSoldier->getCollection() + 0x10 + (pSoldier->getCollection()->offsetCount * 0x50));
                pPlayer->m_pos = D3DXVECTOR3(-trans.trans.x, trans.trans.z, trans.trans.y);
                pPlayer->m_rotation = atan2(trans.forward.z, -trans.forward.x);
            }

            BFBC2::fb::Vec3 speed = { 0, 0, 0 };
            if (pData->getAttachedControllable()) {
                speed = ((BFBC2::ClientVehicleEntity*)pData->getAttachedControllable())->getSpeed();
            } else if (pSoldier->getReplicatedController()) {
                speed = pSoldier->getReplicatedController()->getVelocity();
            }
            pPlayer->m_vel = D3DXVECTOR3(-speed.x, speed.z, speed.y);

            pPlayer->m_health = pSoldier->getHealth() / pSoldier->getMaxHealth();

            UpdateBFBC2Skeleton(&pPlayer->m_skeleton, pSoldier->getPersonView1().animation);

            // check distance of skeleton to soldier position
            if (pPlayer->getSkeleton())
            {
                auto hips = pPlayer->getSkeleton()->getHipsBone();
                if (hips)
                {
                    D3DXVECTOR3 dist = hips->pos - pPlayer->getPos();
                    if (D3DXVec3Length(&dist) > 2.0f)
                        pPlayer->m_skeleton.m_internalRep = nullptr;
                }
            }

            if (isOwn) {
                if (pSoldier->getCurrentWeapon() && pSoldier->getCurrentWeapon()->getData()) {
                    auto weap = pSoldier->getCurrentWeapon()->getData();

                    m_data.m_weapon.m_gravity = 9.81f;

                    if (weap->getWeaponFiring() && weap->getWeaponFiring()->getPrimaryFire()) {
                        m_data.m_weapon.m_bulletSpeed = weap->getWeaponFiring()->getPrimaryFire()->getShot().initialSpeed.z;
                        m_data.m_weapon.m_internalRep = (void*)(pSoldier->getCurrentWeapon());
                    }
                }
            }
        }
    }
}
void BattlefieldDataMgr::UpdateBFBC2()
{
    // is ingame
    m_data.m_isIngame = *m_memsBFBC2.pIsIngame == 0;

    // cam data
    if (*m_memsBFBC2.pCam) {
        D3DXVECTOR3 pos = (*m_memsBFBC2.pCam)->getPos();
        D3DXVECTOR3 viewVec = (*m_memsBFBC2.pCam)->getViewVec();
        m_data.m_camData.m_pos = D3DXVECTOR3(-pos.x, pos.z, pos.y);
        m_data.m_camData.m_viewVec = D3DXVECTOR3(-viewVec.x, viewVec.z, viewVec.y);
        m_data.m_camData.m_internalRep = (void*)(*m_memsBFBC2.pCam);
    } else {
        m_data.m_camData.m_internalRep = nullptr;
    }

    uintptr_t adrFovy = *(uintptr_t*)m_memsBFBC2.fovy;
    if (adrFovy) {
        float tryFovy = *(float*)(adrFovy + 0x30);
        if (tryFovy > 0.05f)
            m_data.m_camData.m_fovy = tryFovy;
    }

    // own data
    if (*m_memsBFBC2.pMystate) {
        m_data.m_ownData.m_reload = static_cast<float>((*m_memsBFBC2.pMystate)->getReload()) / 100.0f;
        m_data.m_ownData.m_overheat = (*m_memsBFBC2.pMystate)->getOverheat();
        m_data.m_ownData.m_spread = (*m_memsBFBC2.pMystate)->getSpread();
        m_data.m_ownData.m_internalRep = (void*)(*m_memsBFBC2.pMystate);
    } else {
        m_data.m_ownData.m_internalRep = nullptr;
    }

    m_data.m_weapon.m_internalRep = nullptr;

    // from context
    m_data.m_internalRep = (void*)m_memsBFBC2.pCtx;
    m_data.m_ownPlayerIndex = -1;
    auto ctx = m_memsBFBC2.pCtx;
    if (ctx->getPlayerMgr())
    {
        auto playerVec = ctx->getPlayerMgr()->getPlayers();

        m_data.m_players.resize(playerVec.size());
        for (size_t i = 0; i < playerVec.size(); i++) {
            bool isOwn = playerVec[i] == ctx->getPlayerMgr()->getLocalPlayer();
            UpdateBFBC2Player(&m_data.m_players[i], playerVec[i], isOwn);
            if (isOwn)
                m_data.m_ownPlayerIndex = i;
        }
    } else {
        m_data.m_players.clear();
    }
}
void __fastcall hkBFBC2InputUpdate(BFBC2::BorderInputNode *pInst, int, float deltaTime)
{
    auto bfmgr = BattlefieldDataMgr::getInstance();

    uintptr_t orgFunc = bfmgr->m_hkBFBC2InputUpdate->getLocation();
    ((void(__thiscall*)(BFBC2::BorderInputNode*, float))(orgFunc))(pInst, deltaTime);

    float yaw;
    float pitch;
    if (bfmgr->hkInputUpdateCam(&yaw, &pitch))
    {
        pInst->updateLevelInCache(BFBC2::InputConceptId::Yaw, -yaw);
        pInst->updateLevelInCache(BFBC2::InputConceptId::Pitch, pitch);
    }
}

void BattlefieldDataMgr::UpdateBF3Player(Player *pPlayer, const BF3::ClientPlayer *pData, bool isOwn)
{
    pPlayer->m_internalRep = nullptr;

    if (pData)
    {
        pPlayer->m_teamId = pData->getTeamId();
        pPlayer->m_name = std::string(pData->getName());

        auto pSoldier = pData->getSoldier();
        if (pSoldier)
        {
            pPlayer->m_internalRep = (void*)pData;

            auto trans = pSoldier->getMeshTransform();
            pPlayer->m_pos = D3DXVECTOR3(-trans.trans.x, trans.trans.z, trans.trans.y);
            pPlayer->m_rotation = atan2(trans.forward.z, -trans.forward.x);

            BF3::fb::Vec3 speed = { 0, 0, 0 };
            if (pData->getAttachedControllable()) {
                speed = ((BF3::ClientVehicleEntity*)pData->getAttachedControllable())->getSpeed();
            } else if (pSoldier->getReplicatedController()) {
                speed = pSoldier->getReplicatedController()->getVelocity();
            }
            pPlayer->m_vel = D3DXVECTOR3(-speed.x, speed.z, speed.y);

            pPlayer->m_health = pSoldier->getHealth() / 100.0f;

            // TODO skeleton
            pPlayer->m_skeleton.m_internalRep = nullptr;

            if (isOwn) {
                // TODO weapon
            }
        }
    }
}
void BattlefieldDataMgr::UpdateBF3()
{
    // is ingame
    m_data.m_isIngame = true;

    // cam data
    auto pCam = m_memsBF3.pCam;
    m_data.m_camData.m_pos = D3DXVECTOR3(-pCam->getPos().x, pCam->getPos().z, pCam->getPos().y);
    m_data.m_camData.m_viewVec = D3DXVECTOR3(pCam->getViewVec().x, -pCam->getViewVec().z, -pCam->getViewVec().y);
    m_data.m_camData.m_fovy = pCam->getFovy();
    m_data.m_camData.m_internalRep = (void*)pCam;

    // own data
    m_data.m_ownData.m_internalRep = nullptr;

    m_data.m_weapon.m_internalRep = nullptr;

    // form context
    m_data.m_internalRep = (void*)m_memsBF3.pCtx;
    m_data.m_ownPlayerIndex = -1;
    auto ctx = m_memsBF3.pCtx;
    if (ctx->getPlayerMgr())
    {
        auto playerVec = ctx->getPlayerMgr()->getPlayers();

        m_data.m_players.resize(playerVec.size());
        for (size_t i = 0; i < playerVec.size(); i++) {
            bool isOwn = playerVec[i] == ctx->getPlayerMgr()->getLocalPlayer();
            UpdateBF3Player(&m_data.m_players[i], playerVec[i], isOwn);
            if (isOwn)
                m_data.m_ownPlayerIndex = i;
        }
    } else {
        m_data.m_players.clear();
    }
}
void __fastcall hkBF3InputUpdate(BF3::BorderInputNode *pInst, int, float deltaTime)
{
    auto bfmgr = BattlefieldDataMgr::getInstance();

    uintptr_t orgFunc = bfmgr->m_hkBFBC2InputUpdate->getLocation();
    ((void(__thiscall*)(BF3::BorderInputNode*, float))(orgFunc))(pInst, deltaTime);

    float yaw;
    float pitch;
    if (bfmgr->hkInputUpdateCam(&yaw, &pitch))
    {
        //pInst->updateLevelInCache(BFBC2::InputConceptId::Yaw, -yaw);
        //pInst->updateLevelInCache(BFBC2::InputConceptId::Pitch, pitch);
    }
}

template <class T> class EntityIterator
{
public:
    class Element
    {
    public:

        Element* m_pFlink;
        Element* m_pBlink;
        __int32  m_Unk1;
        __int32  m_Unk2;

        T* getObject()
        {
            intptr_t pObject = reinterpret_cast<intptr_t>(this);
            pObject -= 0x40;

            return reinterpret_cast<T*>(pObject);
        }
    };

    Element* m_pFirst;
    Element* m_pCurrent;

    EntityIterator(BF4::ClientGameWorld* pGameWorld, intptr_t pTypeInfo)
    {
        //typedef Element* (__thiscall* tGetEntityList)(intptr_t pTypeInfo, BF4::ClientGameWorld* pThis);
        //tGetEntityList lpGetEntityList = reinterpret_cast<tGetEntityList>(OFFSET_GETENTITYLIST);

        //this->m_pFirst = (Element*)(pTypeInfo + (((uintptr_t)pGameWorld->getTypeOffset() + 6) * 2 * 8));
        this->m_pFirst = (Element*)(pTypeInfo + 0x60);
        this->m_pCurrent = this->m_pFirst;
    }



    bool hasNext()
    {
        if (m_pCurrent && m_pCurrent->m_pFlink)
            return true;

        return false;
    }

    Element* first()    { return m_pFirst; }
    Element* front()    { return m_pCurrent; }
    Element* next()
    {
        if (!m_pFirst)
            return nullptr;

        m_pCurrent = m_pCurrent->m_pFlink;

        return m_pCurrent;
    }
};

void BattlefieldDataMgr::UpdateBF4Player(Player *pPlayer, const BF4::ClientPlayer *pData, bool isOwn)
{
    pPlayer->m_internalRep = nullptr;

    if (pData)
    {
        pPlayer->m_teamId = pData->getTeamId();
        pPlayer->m_name = std::string(pData->getName());
        //LOG_DBG("team: %i | name: %s\n", pPlayer->m_teamId, pPlayer->m_name.c_str());

        auto pSoldier = *pData->getCharacter();
        if (pSoldier)
        {
            pPlayer->m_internalRep = (void*)pData;

            auto trans = pSoldier->getMeshTransform();
            //LOG_DBG("trans: %f\n", trans.trans.x, trans.trans.y, trans.trans.z);
            pPlayer->m_pos = D3DXVECTOR3(-trans.trans.x, trans.trans.z, trans.trans.y);
            pPlayer->m_rotation = atan2(trans.forward.z, -trans.forward.x);

            BF4::fb::Vec3 speed = { 0, 0, 0 };
            if (pData->getAttachedControllable()) {
                speed = ((BF4::ClientVehicleEntity*)pData->getAttachedControllable())->getVelocity();
                if (pSoldier->getReplicatedController())
                {
                    auto vehpos = pSoldier->getReplicatedController()->getPosition();
                    pPlayer->m_pos = D3DXVECTOR3(-vehpos.x, vehpos.z, vehpos.y);
                }
            } else if (pSoldier->getReplicatedController()) {
                speed = pSoldier->getReplicatedController()->getVelocity();
            }
            pPlayer->m_vel = D3DXVECTOR3(-speed.x, speed.z, speed.y);

            if (pSoldier->getHealthData())
                pPlayer->m_health = pSoldier->getHealthData()->getHealth() / pSoldier->getHealthData()->getMaxHealth();

            // TODO skeleton
            pPlayer->m_skeleton.m_internalRep = nullptr;

            if (isOwn) {
                // TODO weapon

                m_data.m_weapon.m_gravity = 2.0f;
                m_data.m_weapon.m_bulletSpeed = 4000.0f;
                m_data.m_weapon.m_internalRep = (void*)0x13371337;
            }
        }
    }
}
void BattlefieldDataMgr::UpdateBF4()
{
    // is ingame
    m_data.m_isIngame = true;

    // cam data
    auto pCamHolder = m_memsBF4.pCamHolder;
    if (pCamHolder)
    {
        auto pCam = pCamHolder->getCamData();
        if (pCam)
        {
            D3DXMATRIX vmat = pCam->getViewMat();
            vmat._11 = -vmat._11; // negate x
            vmat._12 = -vmat._12;
            vmat._13 = -vmat._13;
            std::swap(vmat._21, vmat._31); // swap y and z
            std::swap(vmat._22, vmat._32);
            std::swap(vmat._23, vmat._33);
            m_data.m_camData.m_viewMat = vmat;
            m_data.m_camData.m_pos = D3DXVECTOR3(-pCam->getPos().x, pCam->getPos().z, pCam->getPos().y);
            m_data.m_camData.m_viewVec = D3DXVECTOR3(pCam->getViewVec().x, -pCam->getViewVec().z, -pCam->getViewVec().y);
            m_data.m_camData.m_fovy = pCam->getFovy();
            m_data.m_camData.m_internalRep = (void*)pCamHolder;
        }
    }

    // own data
    m_data.m_ownData.m_internalRep = nullptr;

    m_data.m_weapon.m_internalRep = nullptr;

    // form context
    m_data.m_internalRep = (void*)m_memsBF4.pCtx;
    m_data.m_ownPlayerIndex = -1;
    auto ctx = m_memsBF4.pCtx;
    if (ctx->getPlayerMgr())
    {
        auto playerVec = ctx->getPlayerMgr()->getPlayers();

        m_data.m_players.resize(playerVec.size());
        for (size_t i = 0; i < playerVec.size(); i++) {
            bool isOwn = playerVec[i] == ctx->getPlayerMgr()->getLocalPlayer();
            UpdateBF4Player(&m_data.m_players[i], playerVec[i], isOwn);
            if (isOwn)
                m_data.m_ownPlayerIndex = i;
        }
    } else {
        m_data.m_players.clear();
    }
}
void __fastcall hkBF4InputUpdate(BF4::IMouse *pInst, int, float deltaTime)
{
    auto bfmgr = BattlefieldDataMgr::getInstance();

    uintptr_t orgFunc = bfmgr->m_hkBF4InputUpdate->getLocation();
    ((void(__thiscall*)(BF4::IMouse*, float))(orgFunc))(pInst, deltaTime);

    float yaw;
    float pitch;
    if (bfmgr->hkInputUpdateCam(&yaw, &pitch))
    {
        //pInst->updateLevelInCache(BFBC2::InputConceptId::Yaw, -yaw);
        //pInst->updateLevelInCache(BFBC2::InputConceptId::Pitch, pitch);
    }
    bfmgr->BF4InputCacheUpdate();
}

#include "../hacklib/include/hacklib/Rng.h"
hl::Rng g_rng;

void BattlefieldDataMgr::BF4InputCacheUpdate()
{
    auto cache = m_memsBF4.pInput->getInputCache();

    if (!m_data.getOwnPlayer() || !m_data.getOwnPlayer()->isValid())
        return;

    // spotting bot
    static hl::Timer tSpot;
    static double pauseSpot;
    if (tSpot.diff() > pauseSpot)
    {
        tSpot.reset();
        pauseSpot = g_rng.nextReal(3.0, 6.0);

        // only press the button if we are alive
        if (m_data.getOwnPlayer() && m_data.getOwnPlayer()->getHealth() > 0)
        {
            cache->m_conceptCache[BF4::InputConceptId::Spot] = 1.0f;
        }
    }

    // vehicle entry bot
    static hl::Timer tClick;
    static double pauseClick;
    if (GetAsyncKeyState(VK_NUMPAD9) < 0 && tClick.diff() > pauseClick)
    {
        tClick.reset();
        pauseClick = g_rng.nextReal(0.02, 0.05);

        cache->m_conceptCache[BF4::InputConceptId::EnterVehicle] = 1.0f;
    }

    float speed = 3.6f * D3DXVec3Length(&m_data.getOwnPlayer()->getVel());
    if (speed > 200.0f) // basic check to see if we are flying jet
    {
        static bool accel = true;
        static float bottomSpeed = 310.0f;
        static float topSpeed = 315.0f;

        if (accel)
        {
            cache->m_conceptCache[BF4::InputConceptId::MoveFB] = 1.0f;
            cache->m_conceptCache[BF4::InputConceptId::MoveForward] = 1.0f;
            if (speed > topSpeed)
            {
                topSpeed = g_rng.nextReal(313.0f, 318.0f);
                accel = false;
            }
        } else {
            cache->m_conceptCache[BF4::InputConceptId::MoveFB] = -1.0f;
            cache->m_conceptCache[BF4::InputConceptId::MoveBackward] = 1.0f;
            cache->m_conceptCache[BF4::InputConceptId::Crawl] = 1.0f;
            cache->m_conceptCache[BF4::InputConceptId::Brake] = 1.0f;
            if (speed < bottomSpeed)
            {
                bottomSpeed = g_rng.nextReal(307.0f, 312.0f);
                accel = true;
            }
        }
    }

    static bool aimbot = true;
    if (GetAsyncKeyState(VK_NUMPAD7) < 0)
    {
        aimbot = !aimbot;
    }

    // aimbot
    if (aimbot && GetAsyncKeyState(VK_CONTROL) < 0)
    {
        auto cam = m_data.getCamData();
        if (cam)
        {
            auto viewMat = cam->getViewMat();

            // transform target position to camera space. front is -z; right is x; up is y;
            D3DXVECTOR3 targetViewspace;
            D3DXVec3TransformCoord(&targetViewspace, &m_targetPos, &viewMat);

            if (targetViewspace.z < 0) // target is in front half space
            {
                // angle around right axis of camera
                float angleYZ = atan2(targetViewspace.y, -targetViewspace.z);
                // angle around up axis of camera
                float angleXZ = atan2(-targetViewspace.x, -targetViewspace.z);
                // roll towards or away from target depending on location of target in top or bottom half space
                //float yaw = 6.0f * -angleXZ;
                float pitch = 5.0f * -angleYZ;
                //float roll = 5.0f * angleXZ * -copysign(1.0f, targetViewspace.y);
                float roll = 5.0f * angleXZ;
                /*float roll;
                if (angleYZ > 1.0f)
                    roll = 3.0f * angleXZ;
                else
                    roll = 3.0f * -angleXZ;*/
                //cache->m_conceptCache[BF4::InputConceptId::Yaw] = yaw;
                cache->m_conceptCache[BF4::InputConceptId::Pitch] = pitch;
                cache->m_conceptCache[BF4::InputConceptId::Roll] = roll;
            }
        }
    }
}



void BattlefieldDataMgr::UpdateSWBFPlayer(Player *pPlayer, const SWBF::ClientPlayer *pData, bool isOwn)
{
    pPlayer->m_internalRep = nullptr;

    if (pData)
    {
        pPlayer->m_teamId = pData->getTeamId();
        pPlayer->m_name = std::string(pData->getName());
        //LOG_DBG("team: %i | name: %s\n", pPlayer->m_teamId, pPlayer->m_name.c_str());

        auto pSoldier = *pData->getCharacter();
        if (pSoldier)
        {
            pPlayer->m_internalRep = (void*)pData;

            auto trans = pSoldier->getMeshTransform();
            //LOG_DBG("trans: %f\n", trans.trans.x, trans.trans.y, trans.trans.z);
            pPlayer->m_pos = D3DXVECTOR3(-trans.trans.x, trans.trans.z, trans.trans.y);
            pPlayer->m_rotation = atan2(trans.forward.z, -trans.forward.x);

            SWBF::fb::Vec3 speed = { 0, 0, 0 };
            if (pData->getAttachedControllable()) {
                speed = ((SWBF::ClientVehicleEntity*)pData->getAttachedControllable())->getVelocity();
                if (pSoldier->getReplicatedController())
                {
                    auto vehpos = pSoldier->getReplicatedController()->getPosition();
                    pPlayer->m_pos = D3DXVECTOR3(-vehpos.x, vehpos.z, vehpos.y);
                }
            }
            else if (pSoldier->getReplicatedController()) {
                speed = pSoldier->getReplicatedController()->getVelocity();
            }
            pPlayer->m_vel = D3DXVECTOR3(-speed.x, speed.z, speed.y);

            if (pSoldier->getHealthData())
                pPlayer->m_health = pSoldier->getHealthData()->getHealth() / pSoldier->getHealthData()->getMaxHealth();

            // TODO skeleton
            pPlayer->m_skeleton.m_internalRep = nullptr;

            if (isOwn) {
                // TODO weapon

                m_data.m_weapon.m_gravity = 2.0f;
                m_data.m_weapon.m_bulletSpeed = 4000.0f;
                m_data.m_weapon.m_internalRep = (void*)0x13371337;
            }
        }
    }
}
void BattlefieldDataMgr::UpdateSWBF()
{
    // is ingame
    m_data.m_isIngame = true;

    // cam data
    auto pCamHolder = m_memsSWBF.pCamHolder;
    if (pCamHolder)
    {
        auto pCam = pCamHolder->getCamData();
        if (pCam)
        {
            D3DXMATRIX vmat = pCam->getViewMat();
            vmat._11 = -vmat._11; // negate x
            vmat._12 = -vmat._12;
            vmat._13 = -vmat._13;
            std::swap(vmat._21, vmat._31); // swap y and z
            std::swap(vmat._22, vmat._32);
            std::swap(vmat._23, vmat._33);
            m_data.m_camData.m_viewMat = vmat;
            m_data.m_camData.m_pos = D3DXVECTOR3(-pCam->getPos().x, pCam->getPos().z, pCam->getPos().y);
            m_data.m_camData.m_viewVec = D3DXVECTOR3(pCam->getViewVec().x, -pCam->getViewVec().z, -pCam->getViewVec().y);
            m_data.m_camData.m_fovy = pCam->getFovy();
            m_data.m_camData.m_internalRep = (void*)pCamHolder;
        }
    }

    // own data
    m_data.m_ownData.m_internalRep = nullptr;

    m_data.m_weapon.m_internalRep = nullptr;

    // form context
    m_data.m_internalRep = (void*)m_memsSWBF.pCtx;
    m_data.m_ownPlayerIndex = -1;
    auto ctx = m_memsSWBF.pCtx;
    if (ctx->getPlayerMgr())
    {
        auto playerVec = ctx->getPlayerMgr()->getPlayers();

        m_data.m_players.resize(playerVec.size());
        for (size_t i = 0; i < playerVec.size(); i++) {
            bool isOwn = playerVec[i] == ctx->getPlayerMgr()->getLocalPlayer();
            UpdateSWBFPlayer(&m_data.m_players[i], playerVec[i], isOwn);
            if (isOwn)
                m_data.m_ownPlayerIndex = i;
        }
    }
    else {
        m_data.m_players.clear();
    }
}
void __fastcall hkSWBFInputUpdate(SWBF::IMouse *pInst, int, float deltaTime)
{
    auto bfmgr = BattlefieldDataMgr::getInstance();

    uintptr_t orgFunc = bfmgr->m_hkSWBFInputUpdate->getLocation();
    ((void(__thiscall*)(SWBF::IMouse*, float))(orgFunc))(pInst, deltaTime);

    float yaw;
    float pitch;
    if (bfmgr->hkInputUpdateCam(&yaw, &pitch))
    {
        // not implmeneted
    }
}