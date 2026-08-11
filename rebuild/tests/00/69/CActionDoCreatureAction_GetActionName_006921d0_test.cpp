#include <cstdio>

static const char* g_lit = 0;
static int g_len = 0;
static void* g_self = 0;

struct CCharString {
    int d;
    CCharString(const char* lit, int len) {
        g_self = this; g_lit = lit; g_len = len; d = 0x1234;
    }
};

struct CActionDoCreatureAction {
    CCharString GetActionName();
};

CCharString CActionDoCreatureAction::GetActionName()
{
    return CCharString((const char*)0x125c130, -1);
}

int main()
{
    CActionDoCreatureAction act;
    CCharString r = act.GetActionName();
    if (g_lit == (const char*)0x125c130 && g_len == -1 && g_self == &r) {
        printf("006921d0_TEST PASS\n");
    } else {
        printf("FAIL lit=%p len=%d self=%p r=%p\n", g_lit, g_len, g_self, (void*)&r);
    }
    return 0;
}