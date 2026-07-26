#include <cstdio>
struct CGameScriptInterface {};
int g_ActiveGossipCategoriesSize = 0;
int __fastcall CGameScriptInterface_GetActiveGossipCategoriesSize(const CGameScriptInterface* self)
{
    (void)self;
    return g_ActiveGossipCategoriesSize;
}
int main()
{
    CGameScriptInterface obj;
    g_ActiveGossipCategoriesSize = 42;
    int r = CGameScriptInterface_GetActiveGossipCategoriesSize(&obj);
    if (r != 42) { std::printf("FAIL got %d\n", r); return 1; }
    g_ActiveGossipCategoriesSize = 0;
    if (CGameScriptInterface_GetActiveGossipCategoriesSize(&obj) != 0) { std::printf("FAIL zero\n"); return 1; }
    std::printf("CGameScriptInterface_00893040_TEST PASS\n");
    return 0;
}