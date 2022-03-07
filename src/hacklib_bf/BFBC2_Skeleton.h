#ifndef BFBC2_SKELETON_H
#define BFBC2_SKELETON_H

#include "../hacklib/include/hacklib/ImplementMember.h"
#include "d3dx9.h"


namespace BFBC2
{
    struct granny_bone
    {
        const char *Name;
        int PartentIndex;
        uint32_t padding[36];
    };
    struct granny_skeleton
    {
        const char *Name;
        int BoneCount;
        granny_bone *Bones;
    };
    struct granny_world_pose
    {
        unsigned int len;
        D3DXMATRIX *first;
        D3DXMATRIX *last;
    };

    class AnimationSkeleton
    {
        IMPLMEMBER(granny_skeleton*, Skeleton, 0xc);
    };
    class AnimatedModelBase
    {
        IMPLMEMBER(granny_world_pose*, WorldPose, 0x18);
    };

    class AnimatedSkeleton
    {
        IMPLMEMBER(AnimationSkeleton*, Skeleton, 0x50);
        IMPLMEMBER_REL(AnimatedModelBase*, ModelBase, 0, Skeleton);
    };
    class AnimatedSoldier : public AnimatedSkeleton
    {
    };

    struct PersonView
    {
        AnimatedSoldier *animation;
        uint32_t padding[6];
    };
}

#endif
