#include <cstdio>

struct CCharString {
    int dummy;
    CCharString(const char* lit, int len);
};

static const char* g_lit = 0;
static int g_len = 0;
static CCharString* g_self = 0;

CCharString::CCharString(const char* lit, int len)
{
    g_self = this; g_lit = lit; g_len = len;
    this->dummy = 0x1234;
}

struct CActionDoCreatureAction {
    CCharString GetActionName();
};

CCharString CActionDoCreatureAction::GetActionName()
{
    return CCharString((const char*)0x125c110, -1);
}

int main()
{
    CActionDoCreatureAction act;
    CCharString r = act.GetActionName();
    if (g_lit == (const char*)0x125c110 && g_len == -1 && r.dummy == 0x1234)
        printf("00692190_TEST PASS\n");
    else
        printf("FAIL lit=%p len=%d dummy=%x\n", g_lit, g_len, r.dummy);
    return 0;
}