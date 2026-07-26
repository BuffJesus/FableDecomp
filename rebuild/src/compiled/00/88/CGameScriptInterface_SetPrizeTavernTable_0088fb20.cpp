extern unsigned char g_prizeTavernTable;

void __stdcall CGameScriptInterface_SetPrizeTavernTable(bool enabled)
{
    g_prizeTavernTable = (unsigned char)enabled;
}