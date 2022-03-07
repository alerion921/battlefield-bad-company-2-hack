#ifndef CAMDATA_H
#define CAMDATA_H

#include "d3dx9.h"


class CamData
{
    friend class BattlefieldDataMgr;
    friend class BattlefieldData;

public:
    const void *getInternalRep() const;

    D3DXMATRIX getViewMat() const;
    D3DXVECTOR3 getPos() const;
    D3DXVECTOR3 getViewVec() const;
    float getFovY() const;

private:
    void *m_internalRep = nullptr;

    D3DXMATRIX m_viewMat = D3DXMATRIX(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    D3DXVECTOR3 m_pos = D3DXVECTOR3(0, 0, 0);
    D3DXVECTOR3 m_viewVec = D3DXVECTOR3(0, 0, 0);
    float m_fovy = 0;

};

#endif
