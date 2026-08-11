#include <stdio.h>

// Model matching s.cpp but with a real, observable CCharString ctor.
static const char* g_lastStr = 0;
static int g_lastLen = 999;

class CCharString {
public:
    void* p;
    CCharString(const char* s, int len) {
        g_lastStr = s;
        g_lastLen = len;
        p = 0;
    }
};

class CActionDoCreatureAction {
public:
    CCharString GetActionName();
};

static const char* const kName = (const char* const)0x0123c49c;

CCharString CActionDoCreatureAction::GetActionName()
{
    return CCharString(kName, -1);
}

int main()
{
    CActionDoCreatureAction obj;
    CCharString r = obj.GetActionName();
    (void)r;
    if (g_lastStr == (const char*)0x0123c49c && g_lastLen == -1) {
        printf("007a6db0_TEST PASS\n");
    } else {
        printf("FAIL str=%p len=%d\n", (void*)g_lastStr, g_lastLen);
    }
    return 0;
}