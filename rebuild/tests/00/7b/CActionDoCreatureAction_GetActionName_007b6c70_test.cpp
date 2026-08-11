#include <cstdio>

struct CCharString {
    char* p;
    CCharString(const char* s, int len);
};

struct CActionDoCreatureAction {
    CCharString GetActionName();
};

// Provide the ctor body so behavior can be exercised without touching 0x123c8d0.
static const char* g_lastStr = 0;
static int g_lastLen = 0;
CCharString::CCharString(const char* s, int len) {
    g_lastStr = s;
    g_lastLen = len;
    p = (char*)s;
}

CCharString CActionDoCreatureAction::GetActionName()
{
    return CCharString((const char*)0x123c8d0, -1);
}

int main()
{
    CActionDoCreatureAction obj;
    CCharString r = obj.GetActionName();
    bool ok = (g_lastStr == (const char*)0x123c8d0)
           && (g_lastLen == -1)
           && (r.p == (char*)0x123c8d0);
    if (ok) printf("007b6c70_TEST PASS\n");
    else    printf("007b6c70_TEST FAIL str=%p len=%d p=%p\n", g_lastStr, g_lastLen, r.p);
    return 0;
}