struct CGameScriptInterface;
extern long g_bettingAmount;

long __fastcall CGameScriptInterface_GetBettingAmount(CGameScriptInterface* self)
{
    return g_bettingAmount;
}