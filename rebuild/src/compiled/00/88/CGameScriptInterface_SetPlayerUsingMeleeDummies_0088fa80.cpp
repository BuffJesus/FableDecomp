extern unsigned char g_playerUsingMeleeDummies;
unsigned char g_playerUsingMeleeDummies;
void __stdcall CGameScriptInterface_SetPlayerUsingMeleeDummies(unsigned char enabled)
{
    g_playerUsingMeleeDummies = enabled;
}