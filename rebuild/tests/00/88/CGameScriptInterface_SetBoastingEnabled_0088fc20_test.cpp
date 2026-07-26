#include <cstdio>

struct CGameScriptInterface {
    void* vt;
};

unsigned char g_boastingEnabled = 0;

void __fastcall CGameScriptInterface_SetBoastingEnabled(CGameScriptInterface* self, void* edx, bool enabled)
{
    (void)self; (void)edx;
    g_boastingEnabled = (unsigned char)enabled;
}

int main()
{
    CGameScriptInterface obj;
    CGameScriptInterface_SetBoastingEnabled(&obj, 0, true);
    if (g_boastingEnabled != 1) { std::printf("FAIL: expected 1 got %u\n", g_boastingEnabled); return 1; }
    CGameScriptInterface_SetBoastingEnabled(&obj, 0, false);
    if (g_boastingEnabled != 0) { std::printf("FAIL: expected 0 got %u\n", g_boastingEnabled); return 1; }
    std::printf("CGameScriptInterface_0088fc20_TEST PASS\n");
    return 0;
}