#ifndef BATTLEFIELDDATAMGR_H
#define BATTLEFIELDDATAMGR_H

#include "../hacklib/include/hacklib/Hooker.h"
#include "BattlefieldData.h"
#include "BFBC2_Main.h"
#include "BF3_Main.h"
#include "BF4_Main.h"
#include "SWBF_Main.h"
#include <mutex>


class BattlefieldDataMgr
{
public:
    BattlefieldDataMgr();
    static BattlefieldDataMgr *getInstance();

    bool init();
    void update();

    virtual void gameCallback() { }

    const BattlefieldData *getData() const;

    void updateInput(float yaw, float pitch, bool fire);
    void setAimAngles(float yaw, float pitch);
    void setAimPos(const D3DXVECTOR3& pos);
    bool hkInputUpdateCam(float *yaw, float *pitch);

    void BF4InputCacheUpdate();

public:
    const hl::IHook *m_hkBFBC2InputUpdate;
    const hl::IHook *m_hkBF3InputUpdate;
    const hl::IHook *m_hkBF4InputUpdate;
    const hl::IHook *m_hkSWBFInputUpdate;

private:
    bool InitBFBC2();
    bool InitBF3();
    bool InitBF4();
    bool InitSWBF();
    void UpdateBFBC2();
    void UpdateBFBC2Player(Player *pPlayer, const BFBC2::ClientPlayer *pData, bool isOwn);
    void UpdateBFBC2Skeleton(Skeleton *pSkel, const BFBC2::AnimatedSoldier *pData);
    void UpdateBF3();
    void UpdateBF3Player(Player *pPlayer, const BF3::ClientPlayer *pData, bool isOwn);
    void UpdateBF4();
    void UpdateBF4Player(Player *pPlayer, const BF4::ClientPlayer *pData, bool isOwn);
    void UpdateSWBF();
    void UpdateSWBFPlayer(Player *pPlayer, const SWBF::ClientPlayer *pData, bool isOwn);

private:
    hl::Hooker m_hooker;

    BattlefieldData m_data;

    bool m_overwriteInput;
    float m_inputYaw;
    float m_inputPitch;
    D3DXVECTOR3 m_targetPos;
    float m_camYawDiff;
    float m_camPitchDiff;

    enum class Version {
        Unknown,
        BFBC2,
        BF3,
        BF4,
        SWBF
    } m_version;

    struct {
        unsigned char *pIsIngame;
        const BFBC2::ClientGameContext *pCtx;
        const BFBC2::Cam **pCam;
        std::uintptr_t fovy;
        const BFBC2::Mystate **pMystate;
        const BFBC2::BorderInputNode **pInput;
    } m_memsBFBC2;
    struct {
        const BF3::ClientGameContext *pCtx;
        const BF3::Cam *pCam;
        const BF3::BorderInputNode *pInput;
    } m_memsBF3;
    struct {
        const BF4::ClientGameContext *pCtx;
        const BF4::CamHolder *pCamHolder;
        const BF4::BorderInputNode *pInput;
    } m_memsBF4;
    struct {
        const SWBF::ClientGameContext *pCtx;
        const SWBF::CamHolder *pCamHolder;
        const SWBF::BorderInputNode *pInput;
    } m_memsSWBF;

};

#endif
