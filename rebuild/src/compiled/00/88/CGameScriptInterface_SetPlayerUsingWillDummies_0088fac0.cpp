extern unsigned char g_playerUsingWillDummies;

void __stdcall CGameScriptInterface_SetPlayerUsingWillDummies(bool bUsing)
{
    g_playerUsingWillDummies = (unsigned char)bUsing;
}