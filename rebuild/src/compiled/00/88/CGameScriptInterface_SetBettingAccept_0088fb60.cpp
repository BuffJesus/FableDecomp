extern unsigned char g_bettingAccept;
unsigned char g_bettingAccept;

void __stdcall CGameScriptInterface_SetBettingAccept(char accept)
{
    g_bettingAccept = (unsigned char)accept;
}