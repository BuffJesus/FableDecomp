#include <cstdio>

static const char* g_lastStr;
static int g_lastLen;

class CCharString {
public:
    CCharString(const char* s, int n) { g_lastStr = s; g_lastLen = n; p = (void*)s; }
    void* p;
};

class CActionDoCreatureAction {
public:
    CCharString GetActionName();
};

CCharString CActionDoCreatureAction::GetActionName()
{
    return CCharString((const char*)0x01270934, -1);
}

int main()
{
    CActionDoCreatureAction a;
    CCharString r = a.GetActionName();
    if (g_lastStr == (const char*)0x01270934 && g_lastLen == -1 && r.p == (void*)0x01270934)
        printf("GAN_OK str=%p len=%d\n", g_lastStr, g_lastLen);
    else
        printf("GAN_FAIL\n");
    return 0;
}