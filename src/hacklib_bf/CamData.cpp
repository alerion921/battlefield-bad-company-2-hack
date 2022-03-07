#include "CamData.h"


const void *CamData::getInternalRep() const
{
    return m_internalRep;
}


D3DXMATRIX CamData::getViewMat() const
{
    return m_viewMat;
}

D3DXVECTOR3 CamData::getPos() const
{
    return m_pos;
}

D3DXVECTOR3 CamData::getViewVec() const
{
    return m_viewVec;
}

float CamData::getFovY() const
{
    return m_fovy;
}
