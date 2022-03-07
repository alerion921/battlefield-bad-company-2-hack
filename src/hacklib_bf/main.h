#ifndef MAIN_H
#define MAIN_H

#include "Hack.h"

#define VERSION_S               "V9.0r0024"


#include "../hacklib/include/hacklib/Main.h"


class BFMain *GetMain();


class BFMain : public hl::Main
{
public:
    Hack *getHack()
    {
        return m_hack;
    }

    bool init() override
    {
        m_hack = new Hack;

        return m_hack->init();
    }
    bool step() override
    {
        return m_hack->runStep();
    }
    void shutdown() override
    {
        delete m_hack;
    }

private:
    Hack *m_hack;

};


#define LOG_INFO(format,...) GetMain()->getHack()->getCon()->printf(format, __VA_ARGS__)
#ifdef _DEBUG
void LogError(const char *file, const char *func, int line, const char *format, ...);
void LogDebug(const char *file, const char *func, int line, const char *format, ...);
#define LOG_DBG(format,...) LogDebug(__FILE__, __FUNCTION__, __LINE__, format, __VA_ARGS__)
#define LOG_ERR(format,...) LogError(__FILE__, __FUNCTION__, __LINE__, format, __VA_ARGS__)
#else
void LogError(const char *format, ...);
#define LOG_DBG(format,...) 
#define LOG_ERR(format,...) LogError(format, __VA_ARGS__);
#endif

#endif
