#include <cstdio>

struct CGSISetScreenMessagesEnabled_Manager
{
    unsigned char m_Pad00[0x2BB];
    unsigned char m_Before;
    bool m_AreScreenMessagesEnabled;
    unsigned char m_After;
};

CGSISetScreenMessagesEnabled_Manager*
    CGSISetScreenMessagesEnabled_ManagerInstance;

class CGameScriptInterface
{
public:
    virtual void SetScreenMessagesEnabled(bool enabled) const;
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
        call CGameScriptInterface::SetScreenMessagesEnabled
    }
}

static int Fail(const char* pMessage)
{
    std::printf("FSE2_008922d0_TEST FAIL: %s\n", pMessage);
    return 1;
}

int main()
{
    CGameScriptInterface interfaceObject;
    CGSISetScreenMessagesEnabled_Manager first = {};
    CGSISetScreenMessagesEnabled_Manager second = {};

    first.m_Before = 0x31;
    first.m_AreScreenMessagesEnabled = true;
    first.m_After = 0x13;
    CGSISetScreenMessagesEnabled_ManagerInstance = &first;
    interfaceObject.CGameScriptInterface::
        SetScreenMessagesEnabled(false);
    if (first.m_AreScreenMessagesEnabled ||
        first.m_Before != 0x31 || first.m_After != 0x13)
        return Fail("false store");

    second.m_Before = 0xC7;
    second.m_AreScreenMessagesEnabled = false;
    second.m_After = 0x7C;
    CGSISetScreenMessagesEnabled_ManagerInstance = &second;
    interfaceObject.CGameScriptInterface::
        SetScreenMessagesEnabled(true);
    if (!second.m_AreScreenMessagesEnabled ||
        second.m_Before != 0xC7 || second.m_After != 0x7C)
        return Fail("true store or manager switch");

    CallWithRawBool(&interfaceObject, 0x96);
    if (*reinterpret_cast<unsigned char*>(
            &second.m_AreScreenMessagesEnabled) != 0x96)
        return Fail("raw ABI byte");
    if (second.m_Before != 0xC7 || second.m_After != 0x7C)
        return Fail("adjacent bytes");

    std::printf("FSE2_008922d0_TEST PASS\n");
    return 0;
}
