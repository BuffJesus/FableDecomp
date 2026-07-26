#include <cstdio>

unsigned char g_bettingAccept;

void __stdcall CGameScriptInterface_SetBettingAccept(char accept)
{
    g_bettingAccept = (unsigned char)accept;
}

int main()
{
    g_bettingAccept = 0;
    CGameScriptInterface_SetBettingAccept(1);
    if (g_bettingAccept != 1) { std::printf("FAIL set 1\n"); return 1; }
    CGameScriptInterface_SetBettingAccept(0);
    if (g_bettingAccept != 0) { std::printf("FAIL set 0\n"); return 1; }
    CGameScriptInterface_SetBettingAccept((char)0x7F);
    if (g_bettingAccept != 0x7F) { std::printf("FAIL set 7F\n"); return 1; }
    std::printf("CGameScriptInterface_0088fb60_TEST PASS\n");
    return 0;
}