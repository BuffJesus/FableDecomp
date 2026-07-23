#include <cstdio>

unsigned char g_CheapHeadLooking;

struct CGameScriptInterface;

bool __fastcall CGameScriptInterface_GetCheapHeadLooking(const CGameScriptInterface* self)
{
    (void)self;
    return g_CheapHeadLooking != 0;
}

int main()
{
    g_CheapHeadLooking = 0;
    if (CGameScriptInterface_GetCheapHeadLooking(0) != false) { std::printf("FAIL zero\n"); return 1; }
    g_CheapHeadLooking = 1;
    if (CGameScriptInterface_GetCheapHeadLooking(0) != true) { std::printf("FAIL one\n"); return 1; }
    g_CheapHeadLooking = 0xFF;
    if (CGameScriptInterface_GetCheapHeadLooking(0) != true) { std::printf("FAIL ff\n"); return 1; }
    std::printf("CGameScriptInterface_0088faf0_TEST PASS\n");
    return 0;
}