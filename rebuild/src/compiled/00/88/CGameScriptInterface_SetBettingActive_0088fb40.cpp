extern unsigned char g_bettingActive;
void __stdcall CGameScriptInterface_SetBettingActive(bool active)
{
    g_bettingActive = (unsigned char)active;
}