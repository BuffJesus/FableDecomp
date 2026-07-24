#include <cstdio>

struct CHud { int calls_enable; int calls_disable; };
static CHud g_hud_obj = {0,0};
CHud* g_hud = &g_hud_obj;

void __fastcall Hud_Enable(CHud* self) { self->calls_enable++; }
void __fastcall Hud_Disable(CHud* self) { self->calls_disable++; }

void __stdcall CGameScriptInterface_SetHUDEnabled(char enabled)
{
    if (enabled)
        Hud_Enable(g_hud);
    else
        Hud_Disable(g_hud);
}

int main()
{
    CGameScriptInterface_SetHUDEnabled(1);
    CGameScriptInterface_SetHUDEnabled(0);
    CGameScriptInterface_SetHUDEnabled(1);
    if (g_hud_obj.calls_enable == 2 && g_hud_obj.calls_disable == 1) {
        std::printf("CGameScriptInterface_00892050_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL enable=%d disable=%d\n", g_hud_obj.calls_enable, g_hud_obj.calls_disable);
    return 1;
}