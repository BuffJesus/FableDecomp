#include <cstdio>

struct CGSISetCutsceneSkippable_Manager
{
    unsigned char m_Pad00[0xF8];
    bool m_IsSkippable;
    unsigned char m_After;
};

CGSISetCutsceneSkippable_Manager*
    CGSISetCutsceneSkippable_ManagerInstance;

class CGameScriptInterface
{
public:
    virtual void SetCutsceneSkippable(bool isSkippable) const;
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
        call CGameScriptInterface::SetCutsceneSkippable
    }
}

static int Fail(const char* pMessage)
{
    std::printf("FSE2_00892280_TEST FAIL: %s\n", pMessage);
    return 1;
}

int main()
{
    CGameScriptInterface interfaceObject;
    CGSISetCutsceneSkippable_Manager first = {};
    CGSISetCutsceneSkippable_Manager second = {};

    first.m_IsSkippable = true;
    first.m_After = 0x5A;
    CGSISetCutsceneSkippable_ManagerInstance = &first;
    interfaceObject.CGameScriptInterface::SetCutsceneSkippable(false);
    if (first.m_IsSkippable || first.m_After != 0x5A)
        return Fail("false store");

    second.m_IsSkippable = false;
    second.m_After = 0xA5;
    CGSISetCutsceneSkippable_ManagerInstance = &second;
    interfaceObject.CGameScriptInterface::SetCutsceneSkippable(true);
    if (!second.m_IsSkippable || second.m_After != 0xA5)
        return Fail("true store or manager switch");

    CallWithRawBool(&interfaceObject, 0x7E);
    if (*reinterpret_cast<unsigned char*>(&second.m_IsSkippable) != 0x7E)
        return Fail("raw ABI byte");
    if (second.m_After != 0xA5)
        return Fail("adjacent byte");

    std::printf("FSE2_00892280_TEST PASS\n");
    return 0;
}
