#ifndef HACKLIB_MAIN_H
#define HACKLIB_MAIN_H

#include "../hacklib/Handles.h"
#include "../hacklib/MessageBox.h"
#include <string>
#include <memory>


namespace hl
{


/*
 * This class can be used to define behaviour of a dynamic library inside a foreign process.
 * Use the StaticInit helper for actual initialization.
 */
class Main
{
public:
    virtual ~Main() { }

    // Is called on initialization. If returning false, the dll will detach.
    // The default implementation just returns true.
    virtual bool init();
    // Is called continuously sequenially while running. If returning false, the dll will detach.
    // The default implementation sleeps for 10 milliseconds and returns true.
    virtual bool step();
    // Is called on shutdown. Is still called when init returns false.
    // The default implementation does nothing.
    virtual void shutdown();

};

// Returns the module handle to the own dynamic library.
hl::ModuleHandle GetCurrentModule();

// Returns the abolute path with filename to the own dynamic library.
std::string GetCurrentModulePath();

// Implementation detail.
class StaticInitImpl
{
public:
    StaticInitImpl();
    void mainThread();
protected:
    virtual std::unique_ptr<hl::Main> makeMain() const = 0;
private:
    void runMainThread();
    void unloadSelf();
protected:
    hl::Main *m_pMain = nullptr;
};

/*
 * Helper for running a program defined by hl::Main. Make sure to define it once in your dynamic library:
 * hl::StaticInit<MyMain> g_main;
 */
template <typename T>
class StaticInit : private StaticInitImpl
{
public:
    T* getMain()
    {
        return dynamic_cast<T*>(m_pMain);
    }
    const T* getMain() const
    {
        return dynamic_cast<T*>(m_pMain);
    }

protected:
    // Override this for non-default constructors.
    virtual std::unique_ptr<hl::Main> makeMain() const override
    {
        return std::make_unique<T>();
    }

};

}

#endif
