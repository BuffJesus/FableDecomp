#include <cstdio>

bool g_SpotTheAdditionBeaten;

struct CGameScriptInterface {};

bool __fastcall CGameScriptInterface_GetSpotTheAdditionBeaten(const CGameScriptInterface* self)
{
    (void)self;
    return g_SpotTheAdditionBeaten;
}

int main()
{
    CGameScriptInterface obj;
    g_SpotTheAdditionBeaten = false;
    if (CGameScriptInterface_GetSpotTheAdditionBeaten(&obj) != false) { std::printf("FAIL zero\n"); return 1; }
    g_SpotTheAdditionBeaten = true;
    if (CGameScriptInterface_GetSpotTheAdditionBeaten(&obj) != true) { std::printf("FAIL one\n"); return 1; }
    std::printf("CGameScriptInterface_0088fbd0_TEST PASS\n");
    return 0;
}