#include <cstdio>
struct CThing;
struct CGameScriptInterface {
    bool CanThingBe_Seen_ByOtherThing(CThing* a, CThing* b);
    bool CanSeeImpl(CThing* a, CThing* b, int mode);
};
static int g_last_mode = -1;
static CThing* g_last_a = 0;
static CThing* g_last_b = 0;
static CGameScriptInterface* g_last_this = 0;
static bool g_ret = false;

bool CGameScriptInterface::CanSeeImpl(CThing* a, CThing* b, int mode)
{
    g_last_this = this; g_last_a = a; g_last_b = b; g_last_mode = mode;
    return g_ret;
}

bool CGameScriptInterface::CanThingBe_Seen_ByOtherThing(CThing* a, CThing* b)
{
    return CanSeeImpl(a, b, 1);
}

int main()
{
    CGameScriptInterface obj;
    CThing* a = (CThing*)0x1000;
    CThing* b = (CThing*)0x2000;
    g_ret = true;
    bool r = obj.CanThingBe_Seen_ByOtherThing(a, b);
    if (!r) { std::printf("FAIL: expected true\n"); return 1; }
    if (g_last_this != &obj || g_last_a != a || g_last_b != b || g_last_mode != 1) { std::printf("FAIL: args wrong\n"); return 1; }
    g_ret = false;
    r = obj.CanThingBe_Seen_ByOtherThing(a, b);
    if (r) { std::printf("FAIL: expected false\n"); return 1; }
    std::printf("CGameScriptInterface_0089cd10_TEST PASS\n");
    return 0;
}