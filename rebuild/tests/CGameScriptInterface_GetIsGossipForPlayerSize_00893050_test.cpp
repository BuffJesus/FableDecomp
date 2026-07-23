#include <cstdio>

int g_IsGossipForPlayerSize = 0;

int __fastcall CGameScriptInterface_GetIsGossipForPlayerSize(void* self)
{
    (void)self;
    return g_IsGossipForPlayerSize;
}

int main()
{
    g_IsGossipForPlayerSize = 42;
    int r = CGameScriptInterface_GetIsGossipForPlayerSize((void*)0);
    if (r != 42) { std::printf("FAIL: got %d\n", r); return 1; }
    g_IsGossipForPlayerSize = -7;
    r = CGameScriptInterface_GetIsGossipForPlayerSize((void*)0);
    if (r != -7) { std::printf("FAIL: got %d\n", r); return 1; }
    std::printf("CGameScriptInterface_00893050_TEST PASS\n");
    return 0;
}