struct CGameManager { char pad[0x29e]; unsigned char displayMoneyBag; };
extern CGameManager* g_gameManager;

void __fastcall CGameScriptInterface_DisplayMoneyBag(void* self, int edx_dummy, unsigned char val)
{
    (void)self; (void)edx_dummy;
    g_gameManager->displayMoneyBag = val;
}