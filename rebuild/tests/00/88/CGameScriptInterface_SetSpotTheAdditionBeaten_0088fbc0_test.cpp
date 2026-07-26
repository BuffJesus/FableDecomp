#include <cstdio>
struct CGameScriptInterface { char pad[0x1010]; };
unsigned char g_spotTheAdditionBeaten;
void __fastcall CGameScriptInterface_SetSpotTheAdditionBeaten(CGameScriptInterface* self, int edx, unsigned char val)
{
    (void)self; (void)edx;
    g_spotTheAdditionBeaten = val;
}
int main()
{
    CGameScriptInterface obj;
    g_spotTheAdditionBeaten = 0;
    CGameScriptInterface_SetSpotTheAdditionBeaten(&obj, 0, 1);
    if (g_spotTheAdditionBeaten != 1) { std::printf("FAIL set 1\n"); return 1; }
    CGameScriptInterface_SetSpotTheAdditionBeaten(&obj, 0, 0);
    if (g_spotTheAdditionBeaten != 0) { std::printf("FAIL set 0\n"); return 1; }
    std::printf("CGameScriptInterface_0088fbc0_TEST PASS\n");
    return 0;
}