/*
HAEKX - external overlay and aimbot for Battlefield
Injectable module / console output / visual overlay / aimbot
by Rafael S.
started: ~ september 2010
*/

#include "main.h"


hl::StaticInit<BFMain> g_initObj;


class BFMain *GetMain()
{
    return g_initObj.getMain();
}