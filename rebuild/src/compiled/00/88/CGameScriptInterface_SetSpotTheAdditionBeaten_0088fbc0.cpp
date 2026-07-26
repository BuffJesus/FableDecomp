struct CGameScriptInterface { char pad[0x1010]; };
extern unsigned char g_spotTheAdditionBeaten;
unsigned char g_spotTheAdditionBeaten;
void __fastcall CGameScriptInterface_SetSpotTheAdditionBeaten(CGameScriptInterface* self, int edx, unsigned char val)
{
    (void)self; (void)edx;
    g_spotTheAdditionBeaten = val;
}