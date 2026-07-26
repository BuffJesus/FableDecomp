struct CHud;
extern CHud* g_hud;
void __fastcall Hud_Enable(CHud* self);
void __fastcall Hud_Disable(CHud* self);

void __stdcall CGameScriptInterface_SetHUDEnabled(char enabled)
{
    if (enabled)
        Hud_Enable(g_hud);
    else
        Hud_Disable(g_hud);
}