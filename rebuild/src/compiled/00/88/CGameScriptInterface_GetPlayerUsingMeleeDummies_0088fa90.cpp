struct CGameScriptInterface { char pad[4]; };
extern bool g_PlayerUsingMeleeDummies;
bool __fastcall CGameScriptInterface_GetPlayerUsingMeleeDummies(const CGameScriptInterface* self)
{
    (void)self;
    return g_PlayerUsingMeleeDummies;
}