#ifndef BFBC2_INPUT_H
#define BFBC2_INPUT_H


namespace BFBC2
{
    enum class InputConceptId
    {
        Yaw = 6,
        Pitch = 7,
        Roll = 8,
        Fire = 9,
    };

    class BorderInputNode
    {
    public:
        virtual void unk00();
        virtual void unk01();
        virtual void unk02();
        virtual void unk03();
        virtual void unk04();
        virtual void unk05();
        virtual void unk06();
        virtual void unk07();
        virtual void unk08();
        virtual void unk09();
        virtual void unk10();
        virtual void unk11();
        virtual void unk12();
        virtual void unk13();
        virtual void unk14();
        virtual void unk15();
        virtual void unk16();
        virtual void unk17();
        virtual void unk18();
        virtual void unk19();
        virtual void update(float deltaTime);
        virtual void unk21();
        virtual void unk22();
        virtual void unk23();
        virtual void unk24();
        virtual bool getLevelFromCache(InputConceptId concept, float *level, bool forceReadCache);
        virtual bool getIsDownFromCache(InputConceptId concept, bool *isDown, bool forceReadCache);
        virtual void updateLevelInCache(InputConceptId concept, float level);
        virtual void updateIsDownInCache(InputConceptId concept, bool isDown, bool negate);
    };
}

#endif
