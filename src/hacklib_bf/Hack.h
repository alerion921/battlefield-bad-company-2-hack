#ifndef HACK_H
#define HACK_H

#include "../hacklib/include/hacklib/ConsoleEx.h"
#include "../hacklib/include/hacklib/WindowOverlay.h"
#include "../hacklib/include/hacklib/Timer.h"
#include "Input.h"
#include "EspDrawer.h"
#include "BattlefieldDataMgr.h"
#include "Aimbot.h"


class Hack : BattlefieldDataMgr
{
public:
    Hack();

    hl::ConsoleEx *getCon();

    bool runStep();
    void run();
    void stop();
    void toggleDistance();
    void toggleNames();

    virtual void gameCallback();

    bool init();
private:
    //bool init();
    void step();
    void processGameData();

private:
    bool m_running;
    bool m_showDistance;
    bool m_showNames;

    hl::ConsoleEx m_con;
    Input m_input;
    hl::WindowOverlay m_overlay;
    EspDrawer m_drawer;
    Aimbot m_aimbot;
    hl::Timer m_timer;

    const Player *m_target;

};

#endif
