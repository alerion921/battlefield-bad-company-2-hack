#ifndef INPUT_H
#define INPUT_H

#include <array>


class Input
{
public:
    void update();

    bool isDown(int vkey) const;
    bool wentDown(int vkey) const;
    bool wentUp(int vkey) const;

private:
    enum class InputState
    {
        Idle,
        WentDown,
        WentUp
    };

    struct InputStatus
    {
        bool isDown = false;
        InputState state = InputState::Idle;
    };

    std::array<InputStatus, 256> m_status;

};

#endif
