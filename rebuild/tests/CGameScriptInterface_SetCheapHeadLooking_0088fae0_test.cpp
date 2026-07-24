#include <cstdio>
unsigned char g_cheapHeadLooking;
void __stdcall CGameScriptInterface_SetCheapHeadLooking(char val)
{
    g_cheapHeadLooking = (unsigned char)val;
}
int main()
{
    g_cheapHeadLooking = 0;
    CGameScriptInterface_SetCheapHeadLooking(1);
    if (g_cheapHeadLooking != 1) { std::printf("FAIL set1\n"); return 1; }
    CGameScriptInterface_SetCheapHeadLooking(0);
    if (g_cheapHeadLooking != 0) { std::printf("FAIL set0\n"); return 1; }
    CGameScriptInterface_SetCheapHeadLooking((char)0x7F);
    if (g_cheapHeadLooking != 0x7F) { std::printf("FAIL set7F\n"); return 1; }
    std::printf("CGameScriptInterface_0088fae0_TEST PASS\n");
    return 0;
}