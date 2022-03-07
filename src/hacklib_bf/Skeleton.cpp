#include "Skeleton.h"


const void *Skeleton::getInternalRep() const
{
    return m_internalRep;
}


const std::vector<Bone>& Skeleton::getBones() const
{
    return m_bones;
}

const Bone *Skeleton::getHeadBone() const
{
    if (m_headIndex != -1)
        return &m_bones[m_headIndex];
    return nullptr;
}

const Bone *Skeleton::getChestBone() const
{
    if (m_chestIndex != -1)
        return &m_bones[m_chestIndex];
    return nullptr;
}

const Bone *Skeleton::getHipsBone() const
{
    if (m_hipsIndex != -1)
        return &m_bones[m_hipsIndex];
    return nullptr;
}
