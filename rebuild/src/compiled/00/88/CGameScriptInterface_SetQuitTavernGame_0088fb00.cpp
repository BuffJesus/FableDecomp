extern unsigned char g_quitTavernGame;

void __stdcall CGameScriptInterface_SetQuitTavernGame(bool b)
{
    g_quitTavernGame = (unsigned char)b;
}