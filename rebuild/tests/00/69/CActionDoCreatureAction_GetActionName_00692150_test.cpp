#include <cstdio>

static const char* g_lit = 0;
static int g_len = 0;
static void* g_self = 0;

struct CCharString {
    int dummy;
    CCharString(const char* lit, int len);
};

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
    return CCharString((const char*)0x125c0ec, -1);
}

int main()
{
    CActionDoCreatureAction a;
    CCharString r = a.GetActionName();
    if (g_lit == (const char*)0x125c0ec && g_len == -1 && r.dummy == 0x1234)
        printf("00692150_TEST PASS\n");
    else
        printf("FAIL lit=%p len=%d dummy=%x\n", g_lit, g_len, r.dummy);
    return 0;
}