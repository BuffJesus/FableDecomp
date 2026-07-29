#include <cstdio>

struct CGSISetCutsceneSkippablePaused_Manager
{
    unsigned char m_Pad00[0xF8];
    unsigned char m_NormalSkippable;
    bool m_IsSkippableWhilePaused;
    unsigned char m_After;
};

CGSISetCutsceneSkippablePaused_Manager*
    CGSISetCutsceneSkippablePaused_ManagerInstance;

class CGameScriptInterface
{
public:
    virtual void SetCutsceneSkippableWhilePaused(
        bool isSkippableWhilePaused) const;
};

static void CallWithRawBool(
    const CGameScriptInterface* pInterface,
    unsigned char value)
{
    __asm
    {
        mov ecx, pInterface
        movzx eax, value
        push eax
        call CGameScriptInterface::SetCutsceneSkippableWhilePaused
    }
}

static int Fail(const char* pMessage)
{
    std::printf("FSE2_008922a0_TEST FAIL: %s\n", pMessage);
    return 1;
}

int main()
{
    CGameScriptInterface interfaceObject;
    CGSISetCutsceneSkippablePaused_Manager first = {};
    CGSISetCutsceneSkippablePaused_Manager second = {};

    first.m_NormalSkippable = 0x35;
    first.m_IsSkippableWhilePaused = true;
    first.m_After = 0x53;
    CGSISetCutsceneSkippablePaused_ManagerInstance = &first;
    interfaceObject.CGameScriptInterface::
        SetCutsceneSkippableWhilePaused(false);
    if (first.m_IsSkippableWhilePaused ||
        first.m_NormalSkippable != 0x35 || first.m_After != 0x53)
        return Fail("false store");

    second.m_NormalSkippable = 0xCA;
    second.m_IsSkippableWhilePaused = false;
    second.m_After = 0xAC;
    CGSISetCutsceneSkippablePaused_ManagerInstance = &second;
    interfaceObject.CGameScriptInterface::
        SetCutsceneSkippableWhilePaused(true);
    if (!second.m_IsSkippableWhilePaused ||
        second.m_NormalSkippable != 0xCA || second.m_After != 0xAC)
        return Fail("true store or manager switch");

    CallWithRawBool(&interfaceObject, 0x6D);
    if (*reinterpret_cast<unsigned char*>(
            &second.m_IsSkippableWhilePaused) != 0x6D)
        return Fail("raw ABI byte");
    if (second.m_NormalSkippable != 0xCA || second.m_After != 0xAC)
        return Fail("adjacent flags");

    std::printf("FSE2_008922a0_TEST PASS\n");
    return 0;
}
