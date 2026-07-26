#include <cstdio>

struct CGameManager { char pad[0x29e]; unsigned char displayMoneyBag; };
static CGameManager g_inst;
CGameManager* g_gameManager = &g_inst;

void __fastcall CGameScriptInterface_DisplayMoneyBag(void* self, int edx_dummy, unsigned char val)
{
    (void)self; (void)edx_dummy;
    g_gameManager->displayMoneyBag = val;
}

int main()
{
    CGameScriptInterface_DisplayMoneyBag((void*)0x1234, 0, 1);
    if (g_inst.displayMoneyBag != 1) { std::printf("FAIL set 1\n"); return 1; }
    CGameScriptInterface_DisplayMoneyBag((void*)0x1234, 0, 0);
    if (g_inst.displayMoneyBag != 0) { std::printf("FAIL set 0\n"); return 1; }
    CGameScriptInterface_DisplayMoneyBag((void*)0x1234, 0, 0xAB);
    if (g_inst.displayMoneyBag != 0xAB) { std::printf("FAIL set AB\n"); return 1; }
    std::printf("CGameScriptInterface_00891c40_TEST PASS\n");
    return 0;
}