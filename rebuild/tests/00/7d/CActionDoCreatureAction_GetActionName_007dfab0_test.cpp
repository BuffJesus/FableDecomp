#include <cstdio>

struct CCharString {
    char* p;
    CCharString(const char* s, int len);
};

struct CActionDoCreatureAction {
    CCharString GetActionName();
};

static const char* g_lastStr = 0;
static int g_lastLen = 0;
CCharString::CCharString(const char* s, int len) {
    g_lastStr = s;
    g_lastLen = len;
    p = (char*)s;
}

CCharString CActionDoCreatureAction::GetActionName()
{
    return CCharString((const char*)0x123d734, -1);
}

int main()
{
    CActionDoCreatureAction obj;
    CCharString r = obj.GetActionName();
    bool ok = (g_lastStr == (const char*)0x123d734)
           && (g_lastLen == -1)
           && (r.p == (char*)0x123d734);
    if (ok) printf("007dfab0_TEST PASS\n");
    else    printf("007dfab0_TEST FAIL str=%p len=%d p=%p\n", g_lastStr, g_lastLen, r.p);
    return 0;
}