#include "OwnData.h"


const void *OwnData::getInternalRep() const
{
    return m_internalRep;
}


float OwnData::getReload() const
{
    return m_reload;
}

float OwnData::getOverheat() const
{
    return m_overheat;
}

float OwnData::getSpread() const
{
    return m_spread;
}
