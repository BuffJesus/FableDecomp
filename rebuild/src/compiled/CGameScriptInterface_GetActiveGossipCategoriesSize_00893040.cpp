struct CGameScriptInterface {};
extern int g_ActiveGossipCategoriesSize;
int __fastcall CGameScriptInterface_GetActiveGossipCategoriesSize(const CGameScriptInterface* self)
{
    (void)self;
    return g_ActiveGossipCategoriesSize;
}