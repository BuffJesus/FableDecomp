extern int g_bettingAmount;

void __stdcall CGameScriptInterface_SetBettingAmount(int amount)
{
    g_bettingAmount = amount;
}