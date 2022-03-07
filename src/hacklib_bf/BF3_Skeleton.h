#ifndef BF3_SKELETON_H
#define BF3_SKELETON_H

#include "../hacklib/include/hacklib/ImplementMember.h"


namespace BF3
{
    enum class Bone
    {
        LeftShoulder,
        UNK1,
        Pelvis,
        Chest,
        Head,
        RightThigh,
        LeftThigh,
        RightKnee,
        LeftKnee,
        UNK9,
        UNK10,
        UNK11,
        Root
    };

    class ClientAntAnimatableComponent
    {

    };

    class BoneTransformInfo
    {
        fb::LinearTransform transform;
        fb::Vec3 position;
    };
    class ClientBoneCollisionComponent
    {
        IMPLMEMBER(BoneTransformInfo*, BoneTransforms, 0x40);
        IMPLMEMBER(bool, CollisionEnabled, 0x1d0);
        IMPLMEMBER_REL(bool, CollisionUpdated, 0, CollisionEnabled);
    };
}

#endif