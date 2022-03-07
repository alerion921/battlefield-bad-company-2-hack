#ifndef SWBF_INPUT_H
#define SWBF_INPUT_H


namespace SWBF
{
    enum InputConceptId
    {

    };

    class IMouse
    {
        IMPLVTFUNC_OR(void, update, 3, float, dt);
    };

    class InputCache
    {

    };

    class BorderInputNode
    {
        IMPLMEMBER(InputCache*, InputCache, 0x8);
        IMPLMEMBER(IMouse*, Mouse, 0x58);
    };
}

#endif