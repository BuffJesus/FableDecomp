extern bool g_playerUsingRangedDummies;
struct CGameScriptInterface { int _dummy; };
bool __fastcall CGameScriptInterface_GetPlayerUsingRangedDummies(CGameScriptInterface* self)
{
    return g_playerUsingRangedDummies;
}