#include <cstdio>

bool g_boastingEnabled = false;

bool __fastcall CGameScriptInterface_GetBoastingEnabled(void* self)
{
    (void)self;
    return g_boastingEnabled;
}

int main()
{
    g_boastingEnabled = false;
    if (CGameScriptInterface_GetBoastingEnabled(0) != false) { std::printf("FAIL zero\n"); return 1; }
    g_boastingEnabled = true;
    if (CGameScriptInterface_GetBoastingEnabled(0) != true) { std::printf("FAIL one\n"); return 1; }
    std::printf("CGameScriptInterface_0088fc30_TEST PASS\n");
    return 0;
}