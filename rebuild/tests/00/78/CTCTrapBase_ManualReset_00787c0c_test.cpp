#include <cstdio>

class CAIStateGroup_MinionBehaviourCrowdBase;

extern "C" void __fastcall Cleanup(CAIStateGroup_MinionBehaviourCrowdBase *self, void *edx);

static int g_cleanupCalled = 0;

extern "C" void __fastcall Cleanup(CAIStateGroup_MinionBehaviourCrowdBase *self, void *edx)
{
    g_cleanupCalled = 1;
}

class CTCTrapBase
{
public:
    char pad[0x58];
    unsigned char m_active;
};

void __fastcall CTCTrapBase_ManualReset(CTCTrapBase *self, void *edx)
{
    if (self->m_active != 0)
    {
        Cleanup((CAIStateGroup_MinionBehaviourCrowdBase*)self, edx);
    }
}

int main()
{
    CTCTrapBase obj;
    for (unsigned i = 0; i < sizeof(obj); ++i) ((char*)&obj)[i] = 0;

    obj.m_active = 0;
    g_cleanupCalled = 0;
    CTCTrapBase_ManualReset(&obj, 0);
    bool zeroPathOk = (g_cleanupCalled == 0);

    obj.m_active = 1;
    g_cleanupCalled = 0;
    CTCTrapBase_ManualReset(&obj, 0);
    bool nonzeroPathOk = (g_cleanupCalled == 1);

    if (zeroPathOk && nonzeroPathOk)
        printf("MANUALRESET_GUARD_OK\n");
    else
        printf("MANUALRESET_GUARD_FAIL\n");
    return 0;
}