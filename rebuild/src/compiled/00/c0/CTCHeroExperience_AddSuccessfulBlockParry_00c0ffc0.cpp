// CTCHeroExperience::AddSuccessfulBlockParry @ 00c0ffc0

// global function pointer at 0x143fdfc (returns a value stored at +4)
extern int (*g_TimerFnPtr)();

struct HelperArg
{
    void MethodC1C830();  // thiscall helper at 0xc1c830, receives arg in ecx
};

struct CTCHeroExperience
{
    HelperArg* field0;
    int field4;

    CTCHeroExperience* AddSuccessfulBlockParry(HelperArg* arg);
};

CTCHeroExperience* CTCHeroExperience::AddSuccessfulBlockParry(HelperArg* arg)
{
    this->field0 = arg;
    arg->MethodC1C830();
    this->field4 = g_TimerFnPtr();
    return this;
}