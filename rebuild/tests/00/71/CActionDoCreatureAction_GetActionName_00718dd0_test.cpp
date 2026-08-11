#include <stdio.h>

class CCharString {
public:
    char* m_p;
    CCharString(const char* s, int n);
};

class CActionDoCreatureAction {
public:
    int dummy;
    CCharString GetActionName();
};

// Capture ctor args
static const char* g_lastStr = 0;
static int g_lastN = 0;

CCharString::CCharString(const char* s, int n)
{
    g_lastStr = s;
    g_lastN = n;
    m_p = (char*)s;
}

CCharString CActionDoCreatureAction::GetActionName()
{
    return CCharString((const char*)0x1265884, -1);
}

int main()
{
    CActionDoCreatureAction obj;
    obj.dummy = 123;
    CCharString r = obj.GetActionName();
    if (g_lastStr == (const char*)0x1265884 && g_lastN == -1 &&
        r.m_p == (char*)0x1265884) {
        printf("00718dd0_TEST PASS\n");
    } else {
        printf("FAIL str=%p n=%d\n", (void*)g_lastStr, g_lastN);
    }
    return 0;
}