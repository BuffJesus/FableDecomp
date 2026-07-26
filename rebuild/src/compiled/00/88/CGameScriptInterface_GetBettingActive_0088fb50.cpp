extern bool g_bettingActive;

bool __fastcall CGameScriptInterface_GetBettingActive(void* self)
{
    (void)self;
    return g_bettingActive;
}