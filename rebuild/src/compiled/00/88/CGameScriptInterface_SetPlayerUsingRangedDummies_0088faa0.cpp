struct CGameScriptInterface;
extern unsigned char g_PlayerUsingRangedDummies;
void __fastcall CGameScriptInterface_SetPlayerUsingRangedDummies(CGameScriptInterface* self, int edx_dummy, unsigned char val)
{
    g_PlayerUsingRangedDummies = val;
}