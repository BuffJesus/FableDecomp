struct CGameScriptInterface;
extern float g_TradingPriceMult;

float __fastcall CGameScriptInterface_GetTradingPriceMult(const CGameScriptInterface* self)
{
    (void)self;
    return g_TradingPriceMult;
}