extern bool g_QuitTavernGame;

struct CGameScriptInterface { char dummy; };

bool __fastcall CGameScriptInterface_GetQuitTavernGame(CGameScriptInterface* self)
{
    (void)self;
    return g_QuitTavernGame;
}