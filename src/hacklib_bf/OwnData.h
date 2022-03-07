#ifndef OWNDATA_H
#define OWNDATA_H



class OwnData
{
    friend class BattlefieldDataMgr;
    friend class BattlefieldData;

public:
    const void *getInternalRep() const;

    float getReload() const;
    float getOverheat() const;
    float getSpread() const;

private:
    void *m_internalRep = nullptr;

    float m_reload = 0;
    float m_overheat = 0;
    float m_spread = 0;

};

#endif
