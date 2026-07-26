extern int g_tradingPriceMult;
int g_tradingPriceMult;

void __stdcall CGameScriptInterface_SetTradingPriceMult(int mult)
{
    g_tradingPriceMult = mult;
}