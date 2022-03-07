#ifndef BFBC2_MYSTATE_H
#define BFBC2_MYSTATE_H

#include "../hacklib/include/hacklib/ImplementMember.h"


namespace BFBC2
{
    class Mystate
    {
        IMPLMEMBER(int, Reload, 0xc8);
        IMPLMEMBER_REL(float, Spread, 0x18, Reload);
        IMPLMEMBER_REL(float, Overheat, 0x58, Spread);
    };
}

#endif
