struct CGameScriptInterface;
extern bool g_countBetMoneyDown;
bool __fastcall CGameScriptInterface_GetCountBetMoneyDown(const CGameScriptInterface* self)
{
    return g_countBetMoneyDown;
}