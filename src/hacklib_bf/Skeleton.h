#ifndef SKELETON_H
#define SKELETON_H

#include "d3dx9.h"
#include <vector>


struct Bone
{
    bool ignore;
    int parent;
    D3DXVECTOR3 pos;
};


class Skeleton
{
    friend class BattlefieldDataMgr;
    friend class Player;

public:
    const void *getInternalRep() const;

    const std::vector<Bone>& getBones() const;
    const Bone *getHeadBone() const;
    const Bone *getChestBone() const;
    const Bone *getHipsBone() const;

private:
    void *m_internalRep = nullptr;

    std::vector<Bone> m_bones;
    int m_headIndex = -1;
    int m_chestIndex = -1;
    int m_hipsIndex = -1;

};

#endif