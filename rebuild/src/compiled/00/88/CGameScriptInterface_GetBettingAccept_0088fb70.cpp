extern bool g_bettingAccept;

bool __fastcall CGameScriptInterface_GetBettingAccept(void* self)
{
    (void)self;
    return g_bettingAccept;
}